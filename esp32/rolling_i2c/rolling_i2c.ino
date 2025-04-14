#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// === Customizable Settings ===
const char* text = "very very long long text text";  // Text to scroll
const int scrollSpeed = 500;        // Delay between scrolls (ms)
const bool loopScroll = true;       // Repeat the scroll animation?

const int lcdAddress = 0x27;        // I2C address
const int lcdCols = 16;
const int lcdRows = 2;
// ==============================

LiquidCrystal_I2C lcd(lcdAddress, lcdCols, lcdRows);

void setup() {
    lcd.init();
    lcd.backlight();
}

void scrollTextRight() {
    int len = strlen(text);
    for (int x = 0; x <= lcdCols; x++) {
        lcd.clear();
        lcd.setCursor(0, 0);

        String var = "";
        for (int i = 0; i < x; i++) {
            var += " ";
        }
        var += text;
        lcd.print(var.substring(0, lcdCols));  // Ensure it doesn't overflow
        delay(scrollSpeed);
    }
}

void appearTextLeft() {
    int len = strlen(text);
    for (int i = len - 1; i >= 0; i--) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(String(text).substring(i, i + lcdCols));
        delay(scrollSpeed);
    }
}

void loop() {
    scrollTextRight();
    appearTextLeft();

    if (!loopScroll) {
        while (true); // Halt
    }
}

