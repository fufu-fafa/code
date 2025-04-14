#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const char* header = "rolling text:";
const char* text = "very very long long text text";  // Text to scroll
const int scrollSpeed = 500;
const int lcdAddress = 0x27;
const int lcdCols = 16;
const int lcdRows = 2;

LiquidCrystal_I2C lcd(lcdAddress, lcdCols, lcdRows);

void setup() {
    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print(header);
}

void scrollTextRightOnRow1() {
    int len = strlen(text);
    for (int x = 0; x <= lcdCols; x++) {
        lcd.setCursor(0, 1); 
        String var = "";
        for (int i = 0; i < x; i++) {
            var += " ";
        }
        var += text;
        lcd.print(var.substring(0, lcdCols));
        delay(scrollSpeed);
    }
}

void appearTextLeftOnRow1() {
    int len = strlen(text);
    for (int i = len - 1; i >= 0; i--) {
        lcd.setCursor(0, 1); 
        lcd.print("                "); // Clear row 1 manually
        lcd.setCursor(0, 1);
        lcd.print(String(text).substring(i, i + lcdCols));
        delay(scrollSpeed);
    }
}

void loop() {
    scrollTextRightOnRow1();
    appearTextLeftOnRow1();
}

