void setup() {
    pinMode(5, OUTPUT);
}

void loop() {
    for (int sybau = 0; sybau <= 255; sybau++) {
        analogWrite(5, sybau);
        delay(15);
    }
    for (int sybau = 255; sybau >= 0; sybau--) {
        analogWrite(5, sybau);
        delay(15);
    }
}
