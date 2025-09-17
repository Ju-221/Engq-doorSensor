#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// WiFi credentials
const char* ssid = "IEEE";
const char* password = "YOUR_WIFI_PASSWORD";

// Discord webhook URL
const char* webhookURL = "YOUR_DISCORD_WEBHOOK_URL";

// Initialize LCD at address 0x27 (common for 16x2 I2C)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.setCursor(attempts % 16, 1); // show progress dots
    lcd.print(".");
    attempts++;
  }

  //todo - add error message if not connected 
  
  Serial.println("\nConnected to Wi-Fi");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected!");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());

  // Send Discord message
  sendDiscordMessage("ESP32 has turned ON and connected to Wi-Fi!");
  lcd.setCursor(0, 1);
  lcd.print("Message sent!");
}

void loop() {
  // Nothing here, only runs once
}

void sendDiscordMessage(String message) {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;

    http.begin(webhookURL); 
    http.addHeader("Content-Type", "application/json");

    String payload = "{\"content\":\"" + message + "\"}";
    int httpResponseCode = http.POST(payload);

    if (httpResponseCode > 0) {
      Serial.println("Message sent successfully!");
    } else {
      Serial.print("Error sending message: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("Wi-Fi not connected, cannot send message.");
  }
}
