#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>

// Replace with your WiFi credentials
const char* ssid = "justsybau";
const char* password = "idonknowthepasswordwhydyouask";

// LCD at I2C address 0x27, size 16x2
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setupWiFi() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
}

void setupTime() {
  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  struct tm timeinfo;
  while (!getLocalTime(&timeinfo)) {
    Serial.println("Waiting for time...");
    delay(1000);
  }
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  setupWiFi();
  setupTime();
}

void loop() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    lcd.setCursor(0, 0);
    lcd.print("Time unavailable ");
    lcd.setCursor(0, 1);
    lcd.print("Check WiFi/NTP  ");
    return;
  }

  char dateStr[17];
  strftime(dateStr, sizeof(dateStr), "%d-%m-%Y", &timeinfo);

  char timeStr[17];
  strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);

  // Centering logic
  int datePadding = (16 - strlen(dateStr)) / 2;
  int timePadding = (16 - strlen(timeStr)) / 2;

  lcd.clear();
  lcd.setCursor(datePadding, 0);
  lcd.print(dateStr);

  lcd.setCursor(timePadding, 1);
  lcd.print(timeStr);

  delay(1000);
}
