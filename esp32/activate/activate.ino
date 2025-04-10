int pins[] = {21, 19, 18, 5, 17, 16, 4, 13};

void setup() {
    for (int x = 0; x <= 7; x++) {
        pinMode(pins[x], OUTPUT);
    }
}

void loop() {
    for (int x = 0; x <= 7; x++) {
        digitalWrite(pins[x], HIGH);
        delay(1000);
    }
    for (int x = 0; x <= 7; x++) {
        digitalWrite(pins[x], LOW);
        delay(1000);
    }
}
