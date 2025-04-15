// === Customizable Settings ===
const char header[] = "rolling text:";
const char* text = "very very long long text text";  // Text to scroll
const int scrollSpeed = 500;        // Delay between scrolls (ms)
const bool loopScroll = true;       // Repeat the scroll animation?

const int lcdAddress = 0x27;        // I2C address
const int lcdCols = 16;
const int lcdRows = 2;

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(lcdAddress, lcdCols, lcdRows);

void setup() {
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print(header);
}

String padLeft(String str, int totalLength) {
    while (str.length() < totalLength) {
        str = " " + str;
    }
    return str;
}

void scrollTextLeft() {
    String paddedText = String("               ") + text + "               ";
    int len = paddedText.length();

    for (int i = 0; i < len - lcdCols + 1; i++) {
        lcd.setCursor(0, 1);
        lcd.print("                ");  
        lcd.setCursor(0, 1);
        lcd.print(paddedText.substring(i, i + lcdCols));
        delay(scrollSpeed);
    }
}

void loop() {
    scrollTextLeft();

    if (!loopScroll) {
        while (true); // Halt
    }
}

