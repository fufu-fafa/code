int even[4] = {2, 4, 6, 8};
int odd[4] = {3, 5, 7, 9};

void setup() {
    for (int x = 2; x <= 9; x++) {
        pinMode(x, OUTPUT);
    }
}

void reset() {
    for (int x = 2; x <= 9; x++) {
        digitalWrite(x, LOW);
    }
}

void loop() {
    for (int x = 0; x <= 3; x++) {
        digitalWrite(even[x], HIGH);
    }
    delay(100);
    reset();
    for (int y = 0; y <= 3; y++) {
        digitalWrite(odd[y], HIGH);
    }
    delay(100);
    reset();
}
