const int pins[4] = {27, 26, 25, 33};
const int delay_ms = 100;

void setup() {
    for (int x = 0; x < 4; x++) {
        pinMode(pins[x], OUTPUT);
    }
}

void loop() {
    digitalWrite(pins[0], HIGH);
    digitalWrite(pins[3], HIGH);
    digitalWrite(pins[1], LOW);
    digitalWrite(pins[2], LOW);
    delay(delay_ms);


    digitalWrite(pins[0], LOW);
    digitalWrite(pins[3], LOW);
    digitalWrite(pins[1], HIGH);
    digitalWrite(pins[2], HIGH);
    delay(delay_ms);
}
