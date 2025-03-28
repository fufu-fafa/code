void setup() {
  // Set pins 2 to 9 as output
  for (int pin = 2; pin <= 9; pin++) {
    pinMode(pin, OUTPUT);
  }
}

void loop() {
  // Fade in
  for (int brightness = 0; brightness <= 255; brightness++) {
    for (int pin = 2; pin <= 9; pin++) {
      analogWrite(pin, brightness);
    }
    delay(5); // Adjust speed of fading
  }

  // Fade out
  for (int brightness = 255; brightness >= 0; brightness--) {
    for (int pin = 2; pin <= 9; pin++) {
      analogWrite(pin, brightness);
    }
    delay(5);
  }
}

