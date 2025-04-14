#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "BluetoothSerial.h"

// === Default Settings ===
String header = "default:";
String text = "edit using bt on ESP32-LCD";
int scrollSpeed = 450;
const bool loopScroll = true;

const int lcdAddress = 0x27;
const int lcdCols = 16;
const int lcdRows = 2;

LiquidCrystal_I2C lcd(lcdAddress, lcdCols, lcdRows);
BluetoothSerial SerialBT;

void setup() {
    Serial.begin(115200);
    SerialBT.begin("ESP32-LCD"); // Bluetooth device name

    lcd.init();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print(header);
}

void scrollTextLeft() {
    String paddedText = "                " + text + "                ";
    int len = paddedText.length();

    for (int i = 0; i < len - lcdCols + 1; i++) {
        lcd.setCursor(0, 1);
        lcd.print("                ");
        lcd.setCursor(0, 1);
        lcd.print(paddedText.substring(i, i + lcdCols));
        delay(scrollSpeed);

        if (SerialBT.available()) break;  // Allow interrupt via BT
    }
}

void handleBluetoothInput() {
    if (SerialBT.available()) {
        String input = SerialBT.readStringUntil('\n');
        input.trim();

        if (input.startsWith("1.")) {
            header = input.substring(2);
            lcd.setCursor(0, 0);
            lcd.print("                ");  // Clear line
            lcd.setCursor(0, 0);
            lcd.print(header);
            SerialBT.println("line 1 updated.");
        } else if (input.startsWith("2.")) {
            text = input.substring(2);
            SerialBT.println("line 2 updated.");
        } else if (input.startsWith("3.")) {
            int newSpeed = input.substring(2).toInt();
            if (newSpeed > 0) {
                scrollSpeed = newSpeed;
                SerialBT.print("Scroll speed updated to ");
                SerialBT.print(scrollSpeed);
                SerialBT.println(" ms.");
            } else {
                SerialBT.println("Invalid scroll speed.");
            }
        } else {
            SerialBT.println("Use 1. or 2. or 3.");
        }
    }
}

void loop() {
    handleBluetoothInput();
    scrollTextLeft();

    if (!loopScroll) {
        while (!SerialBT.available()) delay(100);
    }
}

