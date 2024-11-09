#include <TFT_eSPI.h>
#include <XPT2046_Touchscreen.h>
#include <BleKeyboard.h>  // Biblioteka BLE HID

#define TOUCH_CS 33
#define TOUCH_IRQ 36
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320

TFT_eSPI tft = TFT_eSPI();
XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);
BleKeyboard bleKeyboard("ESP32 Touch HID", "ESP32", 100);

void setup() {
    tft.init();
    tft.setRotation(1);
    tft.fillScreen(TFT_BLACK);
    touch.begin();
    bleKeyboard.begin();

    // Rysowanie sześciu obszarów przycisków na ekranie
    drawButtons();
}

void loop() {
    if (bleKeyboard.isConnected() && touch.touched()) {
        TS_Point p = touch.getPoint();
        
        int x = p.x;
        int y = p.y;

        if (x > 10 && x < 110 && y > 10 && y < 60) {
            // Akcja 1
            bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
        }
        else if (x > 130 && x < 230 && y > 10 && y < 60) {
            // Akcja 2
            bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
        }
        else if (x > 10 && x < 110 && y > 70 && y < 120) {
            // Akcja 3
            bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
        }
        else if (x > 130 && x < 230 && y > 70 && y < 120) {
            // Akcja 4
            bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
        }
        else if (x > 10 && x < 110 && y > 130 && y < 180) {
            // Akcja 5
            bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
        }
        else if (x > 130 && x < 230 && y > 130 && y < 180) {
            // Akcja 6
            bleKeyboard.write(KEY_RETURN);  // Enter
        }
        
        delay(200);  // Zapobiega wielokrotnemu wywołaniu tej samej akcji
    }
}

void drawButtons() {
    tft.fillRect(10, 10, 100, 50, TFT_BLUE);
    tft.fillRect(130, 10, 100, 50, TFT_RED);
    tft.fillRect(10, 70, 100, 50, TFT_GREEN);
    tft.fillRect(130, 70, 100, 50, TFT_YELLOW);
    tft.fillRect(10, 130, 100, 50, TFT_CYAN);
    tft.fillRect(130, 130, 100, 50, TFT_MAGENTA);

    tft.setTextColor(TFT_WHITE);
    tft.drawString("Vol Up", 20, 20);
    tft.drawString("Vol Down", 140, 20);
    tft.drawString("Play/Pause", 20, 80);
    tft.drawString("Next", 140, 80);
    tft.drawString("Prev", 20, 140);
    tft.drawString("Enter", 140, 140);
}