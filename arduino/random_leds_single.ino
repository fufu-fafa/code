void setup() {
    Serial.begin(9600);
    randomSeed(analogRead(0));
    for (int x = 2; x <= 9; x++) {
        pinMode(x, OUTPUT);
    }
}

void loop() {
    int randNum = random(2, 10);
    digitalWrite(randNum, HIGH);
    delay(150);
    digitalWrite(randNum, LOW);
    delay(150);
}
