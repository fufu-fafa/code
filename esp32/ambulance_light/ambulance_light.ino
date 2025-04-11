void setup() {
    pinMode(5, OUTPUT);
    pinMode(18, OUTPUT);
}

void loop() {
    digitalWrite(5, HIGH);
    delay(250);
    digitalWrite(5, LOW);
    digitalWrite(18, HIGH);
    delay(250);
    digitalWrite(18, LOW);
}
