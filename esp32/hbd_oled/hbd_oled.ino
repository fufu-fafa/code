#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ----------------------------- user settings -------------------------------
#define NAME               "te"
#define NAME2              "mp"
#define SHOW_PROGRESS_BAR  0
#define TEMPO_BPM          100      // quarter-note beats per minute
#define BUZZER_ACTIVE_LOW  0        // 1 = module is on when I/O is LOW
#define NOTE_GAP_MS        40       // silence at the end of every note
#define INTRO_MS           1200     // show first line before singing starts

// ----------------------------- pins ----------------------------------------
#define PIN_SDA     21
#define PIN_SCL     22
#define PIN_BUZZER  19
#define PIN_BUTTON  0               // BOOT button

// ----------------------------- display -------------------------------------
#define SCREEN_W    128
#define SCREEN_H    64
#define OLED_ADDR_1 0x3C
#define OLED_ADDR_2 0x3D

#if BUZZER_ACTIVE_LOW
  #define BUZZER_OFF_LEVEL HIGH
#else
  #define BUZZER_OFF_LEVEL LOW
#endif

// After noTone() the core's tone task detaches the pin asynchronously.
// We wait this long, then drive the pin to its "off" level.
#define BUZZER_PARK_DELAY_MS 10

// Do not start an OLED refresh (~25 ms over I2C) if an audio event is due
// sooner than this, so note timing stays tight.
#define RENDER_GUARD_MS 28

// ----------------------------- notes (Hz) ----------------------------------
// Key of F major, range C5..C6 -- loud and clear on small buzzers.
#define REST      0
#define NOTE_C5   523
#define NOTE_D5   587
#define NOTE_E5   659
#define NOTE_F5   698
#define NOTE_G5   784
#define NOTE_A5   880
#define NOTE_AS5  932   // B-flat 5
#define NOTE_C6   1047

// Durations in sixteenth-note units (3/4 time)
#define D_16   1        // sixteenth
#define D_8D   3        // dotted eighth
#define D_4    4        // quarter
#define D_2    8        // half (held line end)
#define D_FERM 16       // final note, held (fermata)

struct Note {
  uint16_t    freq;      // Hz, 0 = rest
  uint8_t     units;     // length in sixteenth notes
  const char *lyric;     // syllable; trailing space = end of word;
                         // "" = held continuation of previous syllable
};

const Note SONG[] = {
  // Line 1: Happy birthday to you
  { NOTE_C5,  D_8D,   "Hap"    },
  { NOTE_C5,  D_16,   "py "    },
  { NOTE_D5,  D_4,    "birth"  },
  { NOTE_C5,  D_4,    "day "   },
  { NOTE_F5,  D_4,    "to "    },
  { NOTE_E5,  D_2,    "you"    },
  // Line 2: Happy birthday to you
  { NOTE_C5,  D_8D,   "Hap"    },
  { NOTE_C5,  D_16,   "py "    },
  { NOTE_D5,  D_4,    "birth"  },
  { NOTE_C5,  D_4,    "day "   },
  { NOTE_G5,  D_4,    "to "    },
  { NOTE_F5,  D_2,    "you"    },
  // Line 3: Happy birthday dear NAME
  { NOTE_C5,  D_8D,   "Hap"    },
  { NOTE_C5,  D_16,   "py "    },
  { NOTE_C6,  D_4,    "birth"  },
  { NOTE_A5,  D_4,    "day "   },
  { NOTE_F5,  D_4,    "dear "  },
  { NOTE_E5,  D_4,    NAME     },
  { NOTE_D5,  D_2,    NAME2    },
  // Line 4: Happy birthday to you
  { NOTE_AS5, D_8D,   "Hap"    },
  { NOTE_AS5, D_16,   "py "    },
  { NOTE_A5,  D_4,    "birth"  },
  { NOTE_F5,  D_4,    "day "   },
  { NOTE_G5,  D_4,    "to "    },
  { NOTE_F5,  D_FERM, "you"    },
};

const uint8_t NUM_NOTES    = sizeof(SONG) / sizeof(SONG[0]);
const uint8_t LINE_START[] = { 0, 6, 12, 19 };   // first note of each line
const uint8_t NUM_LINES    = sizeof(LINE_START) / sizeof(LINE_START[0]);

const uint32_t UNIT_MS = 60000UL / TEMPO_BPM / 4;  // one sixteenth note

// ----------------------------- globals -------------------------------------
Adafruit_SSD1306 display(SCREEN_W, SCREEN_H, &Wire, -1);
bool oledOk = false;

enum SongState : uint8_t { ST_INTRO, ST_PLAYING, ST_DONE };

SongState state      = ST_INTRO;
uint32_t  stateStart = 0;      // when INTRO started
uint8_t   noteIdx    = 0;
uint32_t  noteStart  = 0;
bool      soundOn    = false;
bool      parkPending = false;
uint32_t  parkAt     = 0;
uint32_t  totalMs    = 0;      // whole song length (computed in setup)

uint32_t  lastRender = 0;
bool      forceRender = true;

// lyric scrolling
float     scrollX    = 0.0f;
int8_t    scrollLine = -1;

// button debounce
bool      btnStable    = HIGH;
bool      btnLastRead  = HIGH;
uint32_t  btnLastChange = 0;
#define   BTN_DEBOUNCE_MS 30

// ============================================================================
//  Buzzer
// ============================================================================
void buzzerPark() {
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, BUZZER_OFF_LEVEL);
  parkPending = false;
}

void buzzerSilence(uint32_t now) {
  if (soundOn) {
    noTone(PIN_BUZZER);
    soundOn = false;
  }
  parkPending = true;
  parkAt = now + BUZZER_PARK_DELAY_MS;
}

void buzzerPlay(uint16_t freq) {
  parkPending = false;
  if (freq == 0) {
    buzzerPark();
    return;
  }
  tone(PIN_BUZZER, freq);
  soundOn = true;
}

// ============================================================================
//  Song helpers
// ============================================================================
uint32_t noteMs(uint8_t i) { return (uint32_t)SONG[i].units * UNIT_MS; }

uint8_t lineOf(uint8_t idx) {
  for (int8_t l = NUM_LINES - 1; l >= 0; l--) {
    if (idx >= LINE_START[l]) return (uint8_t)l;
  }
  return 0;
}

uint8_t lineEnd(uint8_t line) {
  return (line + 1 < NUM_LINES) ? LINE_START[line + 1] : NUM_NOTES;
}

// Note whose syllable should be highlighted (skips "" continuation notes)
uint8_t lyricNote(uint8_t idx) {
  uint8_t first = LINE_START[lineOf(idx)];
  while (idx > first && SONG[idx].lyric[0] == '\0') idx--;
  return idx;
}

// Milliseconds elapsed in the song (for the progress bar)
uint32_t songElapsed(uint32_t now) {
  if (state == ST_INTRO) return 0;
  if (state == ST_DONE)  return totalMs;
  uint32_t t = 0;
  for (uint8_t i = 0; i < noteIdx; i++) t += noteMs(i);
  uint32_t inNote = now - noteStart;
  if (inNote > noteMs(noteIdx)) inNote = noteMs(noteIdx);
  return t + inNote;
}

void beginNote(uint32_t t) {
  noteStart = t;
  buzzerPlay(SONG[noteIdx].freq);
  forceRender = true;
}

void startSong() {
  if (soundOn) noTone(PIN_BUZZER);
  soundOn = false;
  parkPending = true;
  parkAt = millis() + BUZZER_PARK_DELAY_MS;
  state = ST_INTRO;
  stateStart = millis();
  noteIdx = 0;
  scrollLine = -1;
  forceRender = true;
  Serial.println(F("Song start"));
}

void updateSong(uint32_t now) {
  if (parkPending && (int32_t)(now - parkAt) >= 0) buzzerPark();

  switch (state) {
    case ST_INTRO:
      if (now - stateStart >= INTRO_MS) {
        state = ST_PLAYING;
        noteIdx = 0;
        beginNote(now);
      }
      break;

    case ST_PLAYING: {
      uint32_t dur = noteMs(noteIdx);
      uint32_t el  = now - noteStart;
      if (soundOn && el + NOTE_GAP_MS >= dur) {
        buzzerSilence(now);              // short gap between notes
      }
      if (el >= dur) {
        uint32_t next = noteStart + dur; // keep tempo drift-free
        if (now - next > 50) next = now; // ...unless we fell far behind
        noteIdx++;
        if (noteIdx >= NUM_NOTES) {
          buzzerSilence(now);
          state = ST_DONE;
          forceRender = true;
          Serial.println(F("Song done - press BOOT to replay"));
        } else {
          beginNote(next);
        }
      }
      break;
    }

    case ST_DONE:
      break;
  }
}

// Time until the next audio event, used to avoid starting a slow OLED
// refresh right before a note needs to change.
uint32_t msUntilAudioEvent(uint32_t now) {
  uint32_t best = 0xFFFFFFFFUL;
  if (parkPending) {
    int32_t d = (int32_t)(parkAt - now);
    best = d > 0 ? (uint32_t)d : 0;
  }
  if (state == ST_INTRO) {
    uint32_t el = now - stateStart;
    uint32_t d = el >= INTRO_MS ? 0 : INTRO_MS - el;
    if (d < best) best = d;
  } else if (state == ST_PLAYING) {
    uint32_t dur = noteMs(noteIdx);
    uint32_t el  = now - noteStart;
    uint32_t evt = soundOn ? (dur > NOTE_GAP_MS ? dur - NOTE_GAP_MS : 0) : dur;
    uint32_t d = el >= evt ? 0 : evt - el;
    if (d < best) best = d;
  }
  return best;
}

// ============================================================================
//  Display
// ============================================================================
#define CHAR_W1   6     // text size 1 cell width
#define CHAR_W2   12    // text size 2 cell width
#define BAR_Y     54
#if SHOW_PROGRESS_BAR
  #define LYRIC_Y 19    // vertically centred in the space above the bar
#else
  #define LYRIC_Y 24    // vertically centred on the whole screen
#endif

// Build the text of one lyric line. If hlNote belongs to this line, returns
// the character start/length of that syllable (without trailing spaces).
size_t buildLine(uint8_t line, int16_t hlNote, char *buf, size_t bufSize,
                 size_t &hlStart, size_t &hlLen) {
  size_t pos = 0;
  hlStart = 0;
  hlLen = 0;
  for (uint8_t i = LINE_START[line]; i < lineEnd(line); i++) {
    const char *s = SONG[i].lyric;
    size_t n = strlen(s);
    if (pos + n >= bufSize) n = bufSize - 1 - pos;
    if ((int16_t)i == hlNote) {
      hlStart = pos;
      hlLen = n;
      while (hlLen > 0 && s[hlLen - 1] == ' ') hlLen--;
    }
    memcpy(buf + pos, s, n);
    pos += n;
  }
  while (pos > 0 && buf[pos - 1] == ' ') pos--;   // trim trailing space
  buf[pos] = '\0';
  return pos;
}

void drawCentered(const char *s, int16_t y, uint8_t size) {
  int16_t w = (int16_t)(strlen(s) * (size == 1 ? CHAR_W1 : CHAR_W2));
  int16_t x = (SCREEN_W - w) / 2;
  if (x < 0) x = 0;
  display.setTextSize(size);
  display.setCursor(x, y);
  display.print(s);
}

// Returns true while the lyric line is still scrolling (needs more frames).
bool drawLyrics() {
  uint8_t line;
  int16_t hl;
  if (state == ST_INTRO) { line = 0; hl = -1; }
  else { hl = lyricNote(noteIdx); line = lineOf((uint8_t)hl); }

  char   txt[96];
  size_t hlStart, hlLen;
  size_t len = buildLine(line, hl, txt, sizeof(txt), hlStart, hlLen);

  int16_t textW = (int16_t)(len * CHAR_W2);
  int16_t baseX;
  float   target;
  if (textW <= SCREEN_W) {
    baseX = (SCREEN_W - textW) / 2;
    target = 0.0f;
  } else {
    // Scroll so the highlighted syllable stays near the centre.
    baseX = 0;
    float center = (hl >= 0) ? (hlStart + hlLen / 2.0f) * CHAR_W2 : 0.0f;
    target = center - SCREEN_W / 2.0f;
    if (target < 0) target = 0;
    if (target > textW - SCREEN_W) target = (float)(textW - SCREEN_W);
  }

  bool animating = false;
  if ((int8_t)line != scrollLine) {
    scrollLine = (int8_t)line;
    scrollX = target;
  } else {
    float diff = target - scrollX;
    if (diff > 0.5f || diff < -0.5f) {
      scrollX += diff * 0.35f;
      animating = true;
    } else {
      scrollX = target;
    }
  }

  int16_t x0 = baseX - (int16_t)lroundf(scrollX);
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(x0, LYRIC_Y);
  display.print(txt);

  if (hl >= 0 && hlLen > 0) {
    // Inverted box behind the current syllable.
    display.fillRect(x0 + (int16_t)(hlStart * CHAR_W2) - 2, LYRIC_Y - 2,
                     (int16_t)(hlLen * CHAR_W2) + 2, 18, SSD1306_INVERSE);
  }

  return animating;
}

void drawProgress(uint32_t now) {
  uint32_t el = songElapsed(now);
  int16_t  fill = (int16_t)((uint64_t)el * (SCREEN_W - 4) / (totalMs ? totalMs : 1));
  display.drawRect(0, BAR_Y, SCREEN_W, 10, SSD1306_WHITE);
  if (fill > 0) display.fillRect(2, BAR_Y + 2, fill, 6, SSD1306_WHITE);
}

void drawCake(uint32_t now) {
  // plate
  display.drawFastHLine(22, 51, 84, SSD1306_WHITE);
  // bottom tier (filled) with decoration dots
  display.fillRoundRect(32, 37, 64, 14, 2, SSD1306_WHITE);
  for (int16_t x = 37; x < 94; x += 7) display.fillCircle(x, 44, 1, SSD1306_BLACK);
  // top tier (outline) with frosting drips
  display.fillRect(42, 27, 44, 10, SSD1306_BLACK);
  display.drawRoundRect(42, 27, 44, 11, 2, SSD1306_WHITE);
  for (int16_t x = 45; x < 84; x += 6) display.drawFastVLine(x, 28, (x / 6) % 2 ? 4 : 2, SSD1306_WHITE);

  // "19" number candles (rows 15..26, sitting on the top tier)
  // "1": 2px stem, flag at top-left, foot
  display.fillRect(56, 15, 2, 12, SSD1306_WHITE);
  display.drawLine(53, 18, 56, 15, SSD1306_WHITE);
  display.drawLine(53, 19, 56, 16, SSD1306_WHITE);
  display.drawFastHLine(53, 26, 8, SSD1306_WHITE);
  // "9": 2px-thick top loop, right stem, curled foot
  display.drawRoundRect(66, 15, 10, 7, 3, SSD1306_WHITE);
  display.drawRoundRect(67, 16, 8, 5, 2, SSD1306_WHITE);
  display.fillRect(74, 20, 2, 6, SSD1306_WHITE);
  display.drawFastHLine(67, 26, 8, SSD1306_WHITE);
  display.drawFastHLine(67, 25, 2, SSD1306_WHITE);

  // wicks + flickering flames
  bool flick = (now / 180) % 2;
  const int16_t cx[2] = { 56, 70 };
  for (uint8_t i = 0; i < 2; i++) {
    int16_t x = cx[i];
    display.drawPixel(x, 14, SSD1306_WHITE);
    int16_t tip = ((i + (flick ? 1 : 0)) % 2) ? 1 : -1;
    display.fillCircle(x, 11, 2, SSD1306_WHITE);
    display.fillTriangle(x - 2, 11, x + 2, 11, x + tip, 8, SSD1306_WHITE);
  }

  drawCentered("Press BOOT to replay", 56, 1);
}

void render(uint32_t now) {
  if (!oledOk) return;

  uint32_t interval = (state == ST_DONE) ? 180 : 50;
  if (!forceRender && now - lastRender < interval) return;
  if (msUntilAudioEvent(now) < RENDER_GUARD_MS) return;   // try again soon

  lastRender = now;
  forceRender = false;

  display.clearDisplay();
  display.setTextWrap(false);
  display.setTextColor(SSD1306_WHITE);

  if (state == ST_DONE) {
    drawCake(now);
  } else {
    if (drawLyrics()) forceRender = true;
#if SHOW_PROGRESS_BAR
    drawProgress(now);
#endif
  }
  display.display();
}

// ============================================================================
//  Button
// ============================================================================
void updateButton(uint32_t now) {
  bool r = digitalRead(PIN_BUTTON);
  if (r != btnLastRead) {
    btnLastRead = r;
    btnLastChange = now;
  }
  if (now - btnLastChange >= BTN_DEBOUNCE_MS && r != btnStable) {
    btnStable = r;
    if (btnStable == LOW) startSong();   // pressed
  }
}

// ============================================================================
//  OLED init with address fallback
// ============================================================================
bool i2cPresent(uint8_t addr) {
  Wire.beginTransmission(addr);
  return Wire.endTransmission() == 0;
}

bool initOled() {
  const uint8_t addrs[2] = { OLED_ADDR_1, OLED_ADDR_2 };
  for (uint8_t i = 0; i < 2; i++) {
    if (!i2cPresent(addrs[i])) continue;
    if (display.begin(SSD1306_SWITCHCAPVCC, addrs[i], true, false)) {
      Serial.printf("OLED found at 0x%02X\n", addrs[i]);
      return true;
    }
  }
  Serial.println(F("OLED not found (0x3C/0x3D) - playing sound only"));
  return false;
}

// ============================================================================
//  Arduino entry points
// ============================================================================
void setup() {
  // Park the buzzer pin first so it stays quiet during boot.
  pinMode(PIN_BUZZER, OUTPUT);
  digitalWrite(PIN_BUZZER, BUZZER_OFF_LEVEL);

  pinMode(PIN_BUTTON, INPUT_PULLUP);

  Serial.begin(115200);

  Wire.begin(PIN_SDA, PIN_SCL);
  Wire.setClock(400000);
  oledOk = initOled();

  for (uint8_t i = 0; i < NUM_NOTES; i++) totalMs += noteMs(i);

  startSong();
}

void loop() {
  uint32_t now = millis();
  updateButton(now);
  updateSong(now);
  render(millis());
}
