#define NUM_LEDS 8  // Number of LEDs
int ledPins[NUM_LEDS] = {2, 3, 4, 5, 6, 7, 8, 9};  // LED pins

void setup() {
    for (int i = 0; i < NUM_LEDS; i++) {
        pinMode(ledPins[i], OUTPUT);
    }
}

void loop() {
    // Move left to right
    for (int i = 0; i < NUM_LEDS; i++) {
        digitalWrite(ledPins[i], HIGH);
        delay(100);
        digitalWrite(ledPins[i], LOW);
    }

    // Move right to left
    for (int i = NUM_LEDS - 1; i >= 0; i--) {
        digitalWrite(ledPins[i], HIGH);
        delay(100);
        digitalWrite(ledPins[i], LOW);
    }
}

