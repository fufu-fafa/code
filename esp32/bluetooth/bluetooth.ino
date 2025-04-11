#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD with I2C address 0x27

String var1 = "config with BT:";
String var2 = "ESP32_LCD_BT"; 

void updateLCD() {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(var1);  // Display var1 on the first line
    lcd.setCursor(0, 1);
    lcd.print(var2);  // Display var2 on the second line
}

void setup() {
    Serial.begin(115200);
    SerialBT.begin("ESP32_LCD_BT"); // Set Bluetooth name
    lcd.init();
    lcd.backlight();
    
    updateLCD();
    Serial.println("Bluetooth is ready. Send '1:text' or '2:text' to update LCD.");
}

void loop() {
    if (SerialBT.available()) {
        String receivedText = SerialBT.readStringUntil('\n'); // Read until newline
        receivedText.trim(); // Remove whitespace

        if (receivedText.startsWith("1:")) {
            var1 = receivedText.substring(2); // Extract new value for var1
            Serial.println("Updated var1: " + var1);
        } else if (receivedText.startsWith("2:")) {
            var2 = receivedText.substring(2); // Extract new value for var2
            Serial.println("Updated var2: " + var2);
        }

        updateLCD(); // Refresh the LCD display
    }
}

