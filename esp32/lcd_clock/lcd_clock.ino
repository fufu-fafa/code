#include <WiFi.h>
#include <LiquidCrystal_I2C.h>
#include <time.h>

const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Time sync tracking
unsigned long lastSyncTime = 0;
const unsigned long syncInterval = 3600000; // 1 hour = 3600000 ms

void setup() {
  Serial.begin(115200);
  delay(100);

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  WiFi.begin(ssid, password);
  int retries = 0;
  while (WiFi.status() != WL_CONNECTED && retries < 20) {
    delay(500);
    Serial.print(".");
    retries++;
  }

  lcd.clear();
  if (WiFi.status() == WL_CONNECTED) {
    lcd.setCursor(0, 0);
    lcd.print("WiFi Connected");
    Serial.println("WiFi Connected");
  } else {
    lcd.setCursor(0, 0);
    lcd.print("WiFi Failed");
    Serial.println("WiFi Connection Failed");
    while (true); // Stop here if no WiFi
  }

  delay(1000);
  lcd.clear();

  configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  lastSyncTime = millis();
}

void loop() {
  // Re-sync NTP every 1 hour
  if (millis() - lastSyncTime > syncInterval) {
    Serial.println("Re-syncing time...");
    configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    lastSyncTime = millis();
  }

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    lcd.setCursor(0, 0);
    lcd.print("Time unavailable ");
    lcd.setCursor(0, 1);
    lcd.print("Check WiFi/NTP  ");
    delay(2000);
    return;
  }

  char dateStr[17];
  strftime(dateStr, sizeof(dateStr), "%d-%m-%Y", &timeinfo);
  int datePadding = (16 - strlen(dateStr)) / 2;

  char timeStr[17];
  strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);
  int timePadding = (16 - strlen(timeStr)) / 2;

  lcd.clear();
  lcd.setCursor(datePadding, 0);
  lcd.print(dateStr);

  lcd.setCursor(timePadding, 1);
  lcd.print(timeStr);

  delay(1000);
}
