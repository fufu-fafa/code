#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(115200);

  Wire.begin(21, 22);
  Wire.setClock(400000);

  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 allocation/init failed. Check wiring/address.");
    while (true) delay(1);
  }

  display.clearDisplay();

  // Text demo
  display.setTextSize(1);                 // 1..8 (scales bitmap font)
  display.setTextColor(SSD1306_WHITE);    // monochrome
  display.setCursor(0, 0);
  display.println(F("Hello, ESP32 + OLED!"));
  display.println(F("SSD1306 128x64 over I2C"));
  display.display();  // Push buffer to the screen
  delay(1500);

  // Simple graphics demo
  display.clearDisplay();
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  display.drawLine(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SSD1306_WHITE);
  display.drawLine(0, SCREEN_HEIGHT-1, SCREEN_WIDTH-1, 0, SSD1306_WHITE);
  display.setCursor(10, 26);
  display.setTextSize(2);
  display.println(F("OLED"));
  display.display();
}

void loop() {
  // Example: tiny counter without flicker (draw to buffer, then display())
  static uint32_t t0 = millis();
  static int count = 0;

  if (millis() - t0 > 500) {
    t0 = millis();
    count++;

    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println(F("Counter:"));

    display.setTextSize(3);
    display.setCursor(0, 20);
    display.printf("%d\n", count);

    display.display();
  }
}
