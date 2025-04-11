#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Use the detected address

void setup() {
    lcd.init();
    lcd.backlight();
}

void loop() {
    for (int x = 0; x <= 15; x++) {
        lcd.clear();
        String var = "";
        for (int y = 0; y < x; y++) {
            var += " ";
        }
        var += "rolling text";
        lcd.setCursor(0, 0);
        lcd.print(var);
        delay(500);
    }

    String word = "olling text";
    for (int i = word.length() - 1; i >= 0; i--) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(word.substring(i));
        delay(500);
    }
}
