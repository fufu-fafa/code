void pwm_s1(int pin1, int pin2) {
    for (int x = 0; x <= 255; x++) {
        analogWrite(pin1, x);
        analogWrite(pin2, 255 - x);
        delay(10);
    }
}

void pwm_s2(int pin1, int pin2) {
    for (int x = 255; x >= 0; x--) {
        analogWrite(pin1, x);
        analogWrite(pin2, 255 - x);
        delay(10);
    }
}

void setup() {
    pinMode(5, OUTPUT);
    pinMode(16, OUTPUT);
}

void loop() {
    pwm_s1(5, 16);
    pwm_s2(5, 16);
}
