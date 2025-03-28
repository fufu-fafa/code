#define DOT_DURATION 150  // Duration of a dot in milliseconds
#define DASH_DURATION (DOT_DURATION * 3)  // Duration of a dash
#define PAUSE_DURATION DOT_DURATION  // Pause between symbols
#define LETTER_PAUSE (DOT_DURATION * 3)  // Pause between letters
#define WORD_PAUSE (DOT_DURATION * 7)  // Pause between words

const int dotPins[] = {5, 6};  // Pins for dots
const int dashPins[] = {2, 3, 4, 5, 6, 7, 8, 9};  // Pins for dashes

const int dotPinCount = sizeof(dotPins) / sizeof(dotPins[0]);
const int dashPinCount = sizeof(dashPins) / sizeof(dashPins[0]);

// Morse code dictionary (dot = '.', dash = '-')
const char* morseAlphabet[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", // A-J
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",   // K-T
  "..-", "...-", ".--", "-..-", "-.--", "--.."                             // U-Z
};

// Message to send in Morse code
const char message[] = "atnynsas";

void setup() {
  // Set dot and dash pins as outputs
  for (int i = 0; i < dotPinCount; i++) {
    pinMode(dotPins[i], OUTPUT);
  }
  for (int i = 0; i < dashPinCount; i++) {
    pinMode(dashPins[i], OUTPUT);
  }
}

void loop() {
  sendMorseMessage(message);
  delay(5000);  // Wait before repeating the message
}

// Function to send a full message
void sendMorseMessage(const char* text) {
  for (int i = 0; text[i] != '\0'; i++) {
    char letter = text[i];

    if (letter == ' ') {
      delay(WORD_PAUSE); // Pause for space between words
    } else {
      sendMorseLetter(letter);
      delay(LETTER_PAUSE); // Pause between letters
    }
  }
}

// Function to send a single letter in Morse code
void sendMorseLetter(char letter) {
  if (letter >= 'a' && letter <= 'z') {
    letter -= 32; // Convert lowercase to uppercase
  }

  if (letter >= 'A' && letter <= 'Z') {
    const char* morseCode = morseAlphabet[letter - 'A'];
    sendMorseCode(morseCode);
  }
}

// Function to send Morse code (dots and dashes)
void sendMorseCode(const char* code) {
  for (int i = 0; code[i] != '\0'; i++) {
    if (code[i] == '.') {
      turnOnDotPins();
      delay(DOT_DURATION);
    } else if (code[i] == '-') {
      turnOnDashPins();
      delay(DASH_DURATION);
    }
    turnOffPins();
    delay(PAUSE_DURATION); // Pause between dots/dashes
  }
}

// Function to turn on only dot pins (5 & 6)
void turnOnDotPins() {
  for (int i = 0; i < dotPinCount; i++) {
    digitalWrite(dotPins[i], HIGH);
  }
}

// Function to turn on all dash pins (2 to 9)
void turnOnDashPins() {
  for (int i = 0; i < dashPinCount; i++) {
    digitalWrite(dashPins[i], HIGH);
  }
}

// Function to turn off all pins
void turnOffPins() {
  for (int i = 0; i < dotPinCount; i++) {
    digitalWrite(dotPins[i], LOW);
  }
  for (int i = 0; i < dashPinCount; i++) {
    digitalWrite(dashPins[i], LOW);
  }
}

