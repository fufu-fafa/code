#include "BluetoothSerial.h"

BluetoothSerial SerialBT;
int corrected;
int len = 5;
float gammaVal = 2.2;

void setup() {
    pinMode(25, OUTPUT);
    Serial.begin(115200);
    SerialBT.begin("esp32_pwm");
    Serial.println("bluetooth is active, connect and send a number for 'len'");
}

void loop() {
    if (SerialBT.available()) {
        String input = SerialBT.readStringUntil('\n');
        int newLen = input.toInt();
        if (newLen > 0 && newLen <= 1000) {
            len = newLen;
            SerialBT.println("updated delay to: " + String(len));
        } else {
            SerialBT.println("invalid input, enter an integer between 1 and 1000.");
        }
    }

    for (int x = 0; x <= 255; x++) {
        corrected = pow((float)x / 255.0, gammaVal) * 255;
        analogWrite(25, corrected);
        delay(len);
    }
    for (int y = 255; y >= 0; y--) {
        corrected = pow((float)y / 255.0, gammaVal) * 255;
        analogWrite(25, corrected);
        delay(len);
    }
}
