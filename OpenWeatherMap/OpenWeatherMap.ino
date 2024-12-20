#include <Arduino.h>
#include <TFT_eSPI.h>
#include <WiFi.h>
#include "config.h"
#include "wiFiManager.h"
#include <HTTPClient.h>
#include <ArduinoJson.h>

// Definicje zmiennych
TFT_eSPI tft = TFT_eSPI();

// URL API do pobierania danych pogodowych
String weatherApiUrl = "http://api.openweathermap.org/data/2.5/weather?lat=" + latitude + "&lon=" + longitude + "&units=metric&appid=" + apiKey;

// Funkcja do pobierania danych z API pogodowego
String getWeatherData() {
  HTTPClient http;
  http.begin(weatherApiUrl);
  
  int httpResponseCode = http.GET();
  String payload = "{}";
  
  if (httpResponseCode > 0) {
    payload = http.getString();
  } else {
    Serial.print("Błąd pobierania danych: ");
    Serial.println(httpResponseCode);
  }
  
  http.end();
  return payload;
}

// Funkcja do wyświetlania danych pogodowych
void displayWeather(String jsonData) {
  StaticJsonDocument<1024> doc;
  deserializeJson(doc, jsonData);
  
  float temperature = doc["main"]["temp"];
  const char* description = doc["weather"][0]["description"];
  float humidity = doc["main"]["humidity"];
  
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);

  tft.setCursor(10, 20);
  tft.printf("Temp: %.1f C", temperature);
  
  tft.setCursor(10, 60);
  tft.printf("Wilgotność: %.0f%%", humidity);

  tft.setCursor(10, 100);
  tft.printf("Opis: %s", description);
  
  // Tu można dodać logikę do wyświetlania mapy w tle
}

// Funkcja ustawienia ekranu
void setupScreen() {
  tft.init();
  tft.setRotation(1);  // Ustawienie orientacji ekranu (1: pozioma)
  tft.fillScreen(TFT_BLACK);
}

// Główna funkcja setup
void setup() {
  Serial.begin(115200);
  
  // Inicjalizacja ekranu
  setupScreen();
  
  // Połączenie z Wi-Fi
  setupWiFiManager();
  
  // Wyświetlenie komunikatu na ekranie
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("Pobieranie danych...");
}

// Główna pętla programu
void loop() {
  loopWiFiManager();
  
  // Pobieranie danych z API
  String weatherData = getWeatherData();
  
  // Wyświetlanie danych na ekranie
  displayWeather(weatherData);
  
  // Aktualizacja co 10 minut
  delay(600000);
}