void setup() {
    for (int x = 2; x <= 9; x++) {
        pinMode(x, OUTPUT);
    }
}

void loop() {
    for (int x = 4; x <= 7; x++) {
        digitalWrite(x, HIGH);
        delay(100);
    }
    digitalWrite(8, HIGH);
    digitalWrite(2, LOW);
    delay(100);
    digitalWrite(9, HIGH);
    digitalWrite(3, LOW);
    delay(100);
    for (int y = 4; y <= 7; y++) {
        digitalWrite(y, LOW);
        delay(100);
    }
    digitalWrite(8, LOW);
    digitalWrite(2, HIGH);
    delay(100);
    digitalWrite(9, LOW);
    digitalWrite(3, HIGH);
    delay(100);
}
