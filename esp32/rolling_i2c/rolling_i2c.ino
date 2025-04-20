#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "BluetoothSerial.h"

LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust if needed
BluetoothSerial SerialBT;

String header = "ESP32 Display";
String text = "config via bluetooth";
String scrollText;
int scrollIndex = 0;

unsigned long previousMillis = 0;
int scrollSpeed = 460; // Default scroll interval

void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32-LCD"); // Bluetooth device name

  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print(header);

  updateScrollText();
}

void loop() {
  handleBluetoothInput();

  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= scrollSpeed) {
    previousMillis = currentMillis;

    // Show scrolling text on line 2
    lcd.setCursor(0, 1);
    lcd.print(scrollText.substring(scrollIndex, scrollIndex + 16));

    scrollIndex++;
    if (scrollIndex > scrollText.length() - 16) {
      scrollIndex = 0;
    }
  }
}

void updateScrollText() {
  scrollText = text + "  " + text + "  ";
  scrollText += scrollText; // Repeat for smooth scroll
  scrollIndex = 0;
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
      SerialBT.println("Line 1 updated.");
    } else if (input.startsWith("2.")) {
      text = input.substring(2);
      updateScrollText();
      SerialBT.println("Line 2 updated.");
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
      SerialBT.println("Use 1. for header, 2. for scroll text, 3. for speed.");
    }
  }
}

