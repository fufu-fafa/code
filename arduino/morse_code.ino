#define DOT_DURATION 200  // Duration of a dot in milliseconds
#define DASH_DURATION (DOT_DURATION * 3)  // Duration of a dash
#define PAUSE_DURATION DOT_DURATION  // Pause between dots/dashes
#define LETTER_PAUSE (DOT_DURATION * 3)  // Pause between letters
#define WORD_PAUSE (DOT_DURATION * 7)  // Pause between words

const int outputPins[] = {2, 3, 4, 5, 6, 7, 8, 9};  // Define pins to be used
const int pinCount = sizeof(outputPins) / sizeof(outputPins[0]); // Count of pins

// Morse code dictionary (dot = '.', dash = '-')
const char* morseAlphabet[] = {
  ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", // A-J
  "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",   // K-T
  "..-", "...-", ".--", "-..-", "-.--", "--.."                             // U-Z
};

// Message to send in Morse code
const char message[] = "SOS HELP";

void setup() {
  for (int i = 0; i < pinCount; i++) {
    pinMode(outputPins[i], OUTPUT);
  }
}

void loop() {
  sendMorseMessage(message);
  delay(5000);  // Wait before repeating the message
}

// Function to send a full message
void sendMorseMessage(const char* text) {
  for (int i = 0; text[i] != '\0'; i++) {
    char letter = text[i];

    if (letter == ' ') {
      delay(WORD_PAUSE); // Pause for space between words
    } else {
      sendMorseLetter(letter);
      delay(LETTER_PAUSE); // Pause between letters
    }
  }
}

// Function to send a single letter in Morse code
void sendMorseLetter(char letter) {
  if (letter >= 'a' && letter <= 'z') {
    letter -= 32; // Convert lowercase to uppercase
  }

  if (letter >= 'A' && letter <= 'Z') {
    const char* morseCode = morseAlphabet[letter - 'A'];
    sendMorseCode(morseCode);
  }
}

// Function to send Morse code (dots and dashes)
void sendMorseCode(const char* code) {
  for (int i = 0; code[i] != '\0'; i++) {
    if (code[i] == '.') {
      turnOnPins();
      delay(DOT_DURATION);
    } else if (code[i] == '-') {
      turnOnPins();
      delay(DASH_DURATION);
    }
    turnOffPins();
    delay(PAUSE_DURATION); // Pause between dots/dashes
  }
}

// Function to turn on all pins
void turnOnPins() {
  for (int i = 0; i < pinCount; i++) {
    digitalWrite(outputPins[i], HIGH);
  }
}

// Function to turn off all pins
void turnOffPins() {
  for (int i = 0; i < pinCount; i++) {
    digitalWrite(outputPins[i], LOW);
  }
}

