#include "wiFiManager.h"

// Funkcja do inicjalizacji ekranu
void initDisplay() {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    tft.setTextColor(TFT_WHITE);
    tft.setTextSize(2);
}

// Funkcja do połączenia z Wi-Fi
void connectToWiFi() {
    WiFi.begin(ssid, password);
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.print("Connecting to WiFi...");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        tft.print(".");
    }
    
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(0, 0);
    tft.print("Connected to WiFi");
    tft.setCursor(0, 20);
    tft.print("IP Address: ");
    tft.print(WiFi.localIP());
}

// Funkcja do ustawienia menedżera Wi-Fi
void setupWiFiManager() {
    initDisplay();
    connectToWiFi();
}

// Funkcja do pętli menedżera Wi-Fi
void loopWiFiManager() {
    // Placeholder for any WiFi manager loop code if needed
}