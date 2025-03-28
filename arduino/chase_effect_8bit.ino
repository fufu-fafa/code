void setup() {
    for (int pin = 2; pin <= 9; pin++) {
        pinMode(pin, OUTPUT);
    }
}

void loop() {
    for (int pin = 2; pin <= 9; pin++) {
        pinMode(pin, OUTPUT);
        digitalWrite(pin, HIGH);
        delay(100);
    }
    for (int pin = 2; pin <= 9; pin++) {
        digitalWrite(pin, LOW);
        delay(100);
    }
}

