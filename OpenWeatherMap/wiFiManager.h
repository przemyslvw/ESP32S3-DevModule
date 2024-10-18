#ifndef WIFIMANAGER_H
#define WIFIMANAGER_H

#include <WiFi.h>
#include <TFT_eSPI.h>
#include "config.h"

// Deklaracje zmiennych
extern TFT_eSPI tft;

// Deklaracje funkcji
void setupWiFiManager();
void loopWiFiManager();

#endif // WIFIMANAGER_H