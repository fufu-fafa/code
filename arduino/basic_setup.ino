void setup() {
    for (int x = 2; x <= 9; x++){
        pinMode(x, OUTPUT);
    }
}

void loop() {
    for (int x = 9; x >= 2; x--) {
        digitalWrite(x, HIGH);
        delay(50);
        digitalWrite(x, LOW);
    }
}
