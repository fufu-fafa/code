#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define date and time manually
int hour   = 21, minute = 8, second = 0;
int day    = 10, month  = 4,  year   = 2025;

// Days in each month (will adjust Feb for leap years)
int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

void setup() {
  Wire.begin();            // initialize I2C
  lcd.init();              // initialize LCD
  lcd.backlight();         // turn on backlight
}

void loop() {
  // 1) Print time on the first line with leading zeros
  lcd.setCursor(0, 0);
  if (hour   < 10) lcd.print('0'); lcd.print(hour);
  lcd.print(':');
  if (minute < 10) lcd.print('0'); lcd.print(minute);
  lcd.print(':');
  if (second < 10) lcd.print('0'); lcd.print(second);

  // 2) Print date on the second line with leading zeros
  lcd.setCursor(0, 1);
  if (day   < 10) lcd.print('0'); lcd.print(day);
  lcd.print('/');
  if (month < 10) lcd.print('0'); lcd.print(month);
  lcd.print('/');
  lcd.print(year);

  // 3) Wait a second
  delay(1000);

  // 4) Increment time
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

  // 5) Adjust February for leap years
  bool isLeap = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
  daysInMonth[1] = isLeap ? 29 : 28;

  // 6) Roll over day→month
  if (day > daysInMonth[month - 1]) {
    day = 1;
    month++;
  }

  // 7) Roll over month→year
  if (month > 12) {
    month = 1;
    year++;
  }
}

