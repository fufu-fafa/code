#include <Wire.h>

void setup() {
    Serial.begin(115200);
    Wire.begin(); // SDA=21, SCL=22 (default for ESP32)

    Serial.println("Scanning for I2C devices...");

    for (uint8_t address = 1; address < 127; address++) {
        Wire.beginTransmission(address);
        if (Wire.endTransmission() == 0) {
            Serial.print("I2C device found at 0x");
            Serial.println(address, HEX);
        }
    }
}

void loop() {
    delay(5000);
}

