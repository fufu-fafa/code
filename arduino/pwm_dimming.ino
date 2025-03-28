const int pwmPins[] = {3, 5, 6, 9};  // PWM-capable pins
const int numPins = sizeof(pwmPins) / sizeof(pwmPins[0]);

void setup() {
    // Set the selected pins as outputs
    for (int i = 0; i < numPins; i++) {
        pinMode(pwmPins[i], OUTPUT);
    }
}

void loop() {
    // Fade in
    for (int brightness = 0; brightness <= 255; brightness++) {
        for (int i = 0; i < numPins; i++) {
            analogWrite(pwmPins[i], brightness);
        }
        delay(5);  // Adjust speed of fading
    }

    // Fade out
    for (int brightness = 255; brightness >= 0; brightness--) {
        for (int i = 0; i < numPins; i++) {
            analogWrite(pwmPins[i], brightness);
        }
        delay(5);
    }
}

