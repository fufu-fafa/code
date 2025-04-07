void setup () {
    Serial.begin(9600);
    randomSeed(analogRead(0));
    for (int x = 2; x <= 9; x++){
        pinMode(x, OUTPUT);
    }
}

void loop () {
    int randNum = random(2, 10);
    for (int x = 2; x <= randNum; x++) {
        digitalWrite(x, HIGH);
    }
    delay(100);
    for (int x = 2; x <= randNum; x++) {
        digitalWrite(x, LOW);
    }
    delay(100);
}
