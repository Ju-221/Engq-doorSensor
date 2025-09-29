#include <WiFi.h>
#include <HTTPClient.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "esp_eap_client.h"


// Eduroam credentials, no need to specify domain if the user is part of the network that the domain belongs to
// This solution uses the EAP WPA2eneterprise solution for eduroam

const char* ssid = "eduroam";         // SSID is always "eduroam"
const char* username = "***";    // your netname username
const char* password = "***";  //your netname pw

// Discord webhook URL
const char* webhookURL = "https://discord.com/api/webhooks/****";

// Initialize LCD at address 0x27 (common for 16x2 I2C)
LiquidCrystal_I2C lcd(0x27, 16, 2);

// GPIO pin to detect circuit
const int sensorPin = 15;
bool lastState = LOW;

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  pinMode(sensorPin, INPUT_PULLUP);

  lcd.setCursor(0, 0);
  lcd.print("Connecting WiFi");

  // Configure WPA2 Enterprise
  connectEduroam();

  //WiFi.begin(ssid);

  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.setCursor(attempts % 16, 1);
    lcd.print(".");
    attempts++;
  }

  Serial.println("\nConnected to Eduroam!");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected!");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());

  sendDiscordMessage("ESP32 connected to Eduroam!");
}

void loop() {
  int state = digitalRead(sensorPin);

  lcd.setCursor(0, 0);
  lcd.print("Door Status:    ");

  if (state == HIGH) {
    if (lastState != HIGH) {
      lcd.setCursor(0, 1);
      lcd.print("CLOSED.         ");
      Serial.println("door is CLOSED");
      sendDiscordMessage("door is now CLOSED!");
      lastState = HIGH;
    }
  } else {
    if (lastState != LOW) {
      lcd.setCursor(0, 1);
      lcd.print("OPEN           ");
      Serial.println("door is OPEN");
      sendDiscordMessage("door is now OPEN!");
      lastState = LOW;
    }
  }

  delay(100);
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

void connectEduroam() {
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);

  // WPA2 Enterprise connection
  WiFi.begin(ssid, WPA2_AUTH_PEAP, username, username, password);

  Serial.print("Connecting to eduroam");
  String period = ".";

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    period += ".";
    Serial.print(period);

    lcd.print(period);
  }
  Serial.println("\nConnected to network!");
  Serial.println(WiFi.localIP());
}
