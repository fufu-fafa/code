void setup() {
    for (int x = 2; x <= 9; x++) {
        pinMode(x, OUTPUT);
    }
}
void loop() {
    digitalWrite(5, HIGH);
    digitalWrite(6, HIGH);
    delay(500);
    digitalWrite(4, HIGH);
    digitalWrite(7, HIGH);
    delay(500);
    digitalWrite(3, HIGH);
    digitalWrite(8, HIGH);
    delay(500);
    digitalWrite(2, HIGH);
    digitalWrite(9, HIGH);
    delay(500);

    digitalWrite(5, LOW);
    digitalWrite(6, LOW);
    delay(500);
    digitalWrite(4, LOW);
    digitalWrite(7, LOW);
    delay(500);
    digitalWrite(3, LOW);
    digitalWrite(8, LOW);
    delay(500);
    digitalWrite(2, LOW);
    digitalWrite(9, LOW);
}
