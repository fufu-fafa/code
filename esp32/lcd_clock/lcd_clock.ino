#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define date and time manually
int hour   = 21, minute = 44, second = 0;
int day    = 10, month  = 4,  year   = 2025;

// Days in each month (will adjust Feb for leap years)
int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

// Timer variables
unsigned long previousMillis = 0;
const unsigned long interval = 1000; // 1 second

void setup() {
  Wire.begin();            
  lcd.init();              
  lcd.backlight();         
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    // 1) Increment time
    second++;
    if (second >= 60) {
      second = 0;
      minute++;
    }
    if (minute >= 60) {
      minute = 0;
      hour++;
    }
    if (hour >= 24) {
      hour = 0;
      day++;
    }

    // 2) Adjust February for leap years
    bool isLeap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
    daysInMonth[1] = isLeap ? 29 : 28;

    // 3) Roll over day→month
    if (day > daysInMonth[month - 1]) {
      day = 1;
      month++;
    }

    // 4) Roll over month→year
    if (month > 12) {
      month = 1;
      year++;
    }

    // 5) Display time
    lcd.setCursor(0, 0);
    if (hour   < 10) lcd.print('0'); lcd.print(hour); lcd.print(':');
    if (minute < 10) lcd.print('0'); lcd.print(minute); lcd.print(':');
    if (second < 10) lcd.print('0'); lcd.print(second);

    lcd.setCursor(0, 1);
    if (day   < 10) lcd.print('0'); lcd.print(day); lcd.print('/');
    if (month < 10) lcd.print('0'); lcd.print(month); lcd.print('/');
    lcd.print(year);
  }
}

