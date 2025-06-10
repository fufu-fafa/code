#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD with I2C address 0x27

String var1 = "config with serial:";
String var2 = "/dev/tty.usbserial-0001"; 

void updateLCD() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(var1);
    lcd.setCursor(0, 1);
    lcd.print(var2);
}

void setup() {
    Serial.begin(115200);
    lcd.init();
    lcd.backlight();
    updateLCD();
}

void loop() {
    if (Serial.available()) {
        String receivedText = Serial.readStringUntil('\n');
        receivedText.trim();
        if (receivedText.startsWith("1.")) var1 = receivedText.substring(2);
        else if (receivedText.startsWith("2.")) var2 = receivedText.substring(2);
        updateLCD();
    }
}

