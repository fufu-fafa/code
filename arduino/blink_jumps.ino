void setup() {
    for (int x = 2; x <= 9; x++) {
        pinMode(x, OUTPUT);
    }
}

void blink_led(int x) {
    digitalWrite(x, HIGH);
    delay(100);
    digitalWrite(x, LOW);
}

void loop() {
    for (int x = 0; x <= 3; x++) {
        blink_led(2 + x);
        blink_led(9 - x);
    }
}
