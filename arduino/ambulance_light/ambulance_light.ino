void setup() {
    for (int x = 2; x <= 9; x++) {
        pinMode(x, OUTPUT);
    }
}

void loop() {
    for (int x = 2; x <= 5; x++) {
        digitalWrite(x, HIGH);
    }
    delay(250);
    for (int x = 2; x <= 5; x++) {
        digitalWrite(x, LOW);
    }
    for (int y = 6; y <= 9; y++) {
        digitalWrite(y, HIGH);
    }
    delay(250);
    for (int y = 6; y <= 9; y++) {
        digitalWrite(y, LOW);
    }
}
