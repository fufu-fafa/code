#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
LiquidCrystal_I2C lcd(0x27, 16, 2); // LCD with I2C address 0x27

String var1 = "Hello";  // First line text
String var2 = "World";  // Second line text

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
    Serial.println("Bluetooth is ready. Send 'V1:text' or 'V2:text' to update LCD.");
}

void loop() {
    if (SerialBT.available()) {
        String receivedText = SerialBT.readStringUntil('\n'); // Read until newline
        receivedText.trim(); // Remove whitespace

        if (receivedText.startsWith("V1:")) {
            var1 = receivedText.substring(3); // Extract new value for var1
            Serial.println("Updated var1: " + var1);
        } else if (receivedText.startsWith("V2:")) {
            var2 = receivedText.substring(3); // Extract new value for var2
            Serial.println("Updated var2: " + var2);
        }

        updateLCD(); // Refresh the LCD display
    }
}

