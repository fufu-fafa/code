const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9};
const int numPins = sizeof(ledPins) / sizeof(ledPins[0]);
const int DELAY_TIME = 300; // Adjust as needed

void setup() {
    for (int i = 0; i < numPins; i++) {
        pinMode(ledPins[i], OUTPUT);
    }
}

void setPinsHighOnly(int pinsToHigh[], int size) {
    // First turn all pins LOW
    for (int i = 0; i < numPins; i++) {
        digitalWrite(ledPins[i], LOW);
    }

    // Then set only the desired pins HIGH
    for (int i = 0; i < size; i++) {
        digitalWrite(pinsToHigh[i], HIGH);
    }

    delay(DELAY_TIME);
}

void loop() {
    int step1[] = {2, 5, 6, 9};
    int step2[] = {4, 5, 6, 7};
    int step3[] = {3, 4, 7, 8};

    setPinsHighOnly(step1, sizeof(step1)/sizeof(step1[0]));
    setPinsHighOnly(step2, sizeof(step2)/sizeof(step2[0]));
    setPinsHighOnly(step3, sizeof(step3)/sizeof(step3[0]));
}

