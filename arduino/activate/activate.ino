void setup() {
    for (int x = 2; x <= 11; x++) {
        pinMode(x, OUTPUT);
        digitalWrite(x, HIGH);
    }
}

void loop() {}
