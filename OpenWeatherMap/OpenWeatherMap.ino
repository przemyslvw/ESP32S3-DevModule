#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>  // Biblioteka do obsługi wyświetlacza TFT (SPI/ILI9341)

// Ustawienia ekranu (przy założeniu, że korzystasz z wyświetlacza TFT ILI9341)
TFT_eSPI tft = TFT_eSPI();

// Stałe do połączenia Wi-Fi
const char* ssid = "Twoja_Nazwa_WiFi";
const char* password = "Twoje_Haslo_WiFi";

// Klucz API do OpenWeatherMap
const String apiKey = "Twoj_Klucz_API";

// Współrzędne lokalizacji (np. szerokość i długość geograficzna)
const String latitude = "52.2297";   // Przykład: Warszawa
const String longitude = "21.0122";

// URL API do pobierania danych pogodowych
String weatherApiUrl = "http://api.openweathermap.org/data/2.5/weather?lat=" + latitude + "&lon=" + longitude + "&units=metric&appid=" + apiKey;

// Funkcja do łączenia z Wi-Fi
#include "WiFiManager.h"

void connectToWiFi() {
  WiFiManager wifiManager;
  wifiManager.autoConnect("AutoConnectAP");

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Połączono z WiFi");
    Serial.print("Adres IP: ");
    Serial.println(WiFi.localIP());
  } else {
    Serial.println("Nie udało się połączyć z WiFi");
  }
}

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
  connectToWiFi();
  
  // Wyświetlenie komunikatu na ekranie
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("Pobieranie danych...");
}

// Główna pętla programu
void loop() {
  // Pobieranie danych z API
  String weatherData = getWeatherData();
  
  // Wyświetlanie danych na ekranie
  displayWeather(weatherData);
  
  // Aktualizacja co 10 minut
  delay(600000);
}
