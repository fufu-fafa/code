int ledPins[] = {21, 19, 18, 5, 17, 16, 4, 13};

void setup() {
  // Set LED pins as output
  for (int i = 0; i < 8; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  for (int count = 0; count <= 255; count++) { // Count up from 0 to 255
    displayBinary(count);
    delay(500);
  }
}

void displayBinary(int num) {
  for (int i = 0; i < 8; i++) {
    digitalWrite(ledPins[i], (num >> i) & 1); // Extract and set each bit
  }
}

