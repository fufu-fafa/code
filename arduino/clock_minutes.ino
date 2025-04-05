const int ledPins[] = {9, 8, 7, 6, 5, 4, 3, 2}; // reversed

void setup() {
    for (int i = 2; i <= 9; i++) {
        pinMode(i, OUTPUT);
    }
}

void loop() {
    for (int count = 0; count <= 255; count++) { 
        displayBinary(count);
        delay(60000);
    }
}

void displayBinary(int num) {
    for (int i = 0; i < 8; i++) {
        digitalWrite(ledPins[i], (num >> i) & 1);
    }
}


