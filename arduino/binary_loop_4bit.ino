// Define LED pins (reversed order)
const int ledPins[] = {5, 4, 3, 2};

void setup() {
  // Set LED pins as output
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
  }
}

void loop() {
  for (int count = 0; count <= 15; count++) { // Count up from 0 to 15
    displayBinary(count);
    delay(500);
  }
}

void displayBinary(int num) {
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], (num >> i) & 1); // Extract and set each bit
  }
}

