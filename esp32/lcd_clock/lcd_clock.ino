#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define date and time manually
int hour = 20, minute = 50, second = 0;
int day = 10, month = 4, year = 2025;

void setup() {
    lcd.init();
    lcd.backlight();
}

void loop() {
    lcd.setCursor(0, 0);
    lcd.print(hour);
    lcd.print(":");
    lcd.print(minute);
    lcd.print(":");
    lcd.print(second);
    
    lcd.setCursor(0, 1);
    lcd.print(day);
    lcd.print("/");
    lcd.print(month);
    lcd.print("/");
    lcd.print(year);
    
    delay(1000);
    second++;
    if (second >= 60) { second = 0; minute++; }
    if (minute >= 60) { minute = 0; hour++; }
    if (hour >= 24) { hour = 0; day++; }
}

