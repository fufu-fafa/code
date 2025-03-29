void setup() {
    for (int pin = 2; pin <= 9; pin++) {
        pinMode(pin, OUTPUT);
    }
    for (int pin = 2; pin <= 9; pin++) {
        digitalWrite(pin, HIGH);
        delay(1000);
    }
}

void loop () {}
