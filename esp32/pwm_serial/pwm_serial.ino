int corrected;
int len = 5;
int minVal = 58;
float gammaVal = 2.2;

void setup() {
    pinMode(25, OUTPUT);
    Serial.begin(115200);
}

void loop() {
    if (Serial.available()) {
        String input = Serial.readStringUntil('\n');
        int newLen = input.toInt();
        if (newLen > 0 && newLen <= 1000) len = newLen;
    }

    for (int x = minVal; x <= 255; x++) {
        corrected = pow((float)x / 255.0, gammaVal) * 255;
        analogWrite(25, corrected);
        delay(len);
    }
    for (int y = 255; y >= minVal; y--) {
        corrected = pow((float)y / 255.0, gammaVal) * 255;
        analogWrite(25, corrected);
        delay(len);
    }
}
