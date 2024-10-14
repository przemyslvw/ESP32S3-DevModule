#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

#define TFT_CYBERPUNK_BG 0x0000 // Black
#define TFT_CYBERPUNK_HOUR 0xF800 // Red
#define TFT_CYBERPUNK_MINUTE 0x07E0 // Green
#define TFT_CYBERPUNK_SECOND 0x0000 // Black
#define TFT_CYBERPUNK_MARKER 0xF81F // Magenta
#define TFT_CYBERPUNK_TEXT 0xFFFF // White

TFT_eSPI tft = TFT_eSPI(); // Invoke custom library

float sx = 0, sy = 1, mx = 1, my = 0, hx = -1, hy = 0; // Saved H, M, S x & y multipliers
float sdeg = 0, mdeg = 0, hdeg = 0;
uint16_t osx = 120, osy = 120, omx = 120, omy = 120, ohx = 120, ohy = 120; // Saved H, M, S x & y coords
uint32_t targetTime = 0; // for next 1 second timeout

static uint8_t conv2d(const char* p); // Forward declaration needed for IDE 1.6.x
uint8_t hh = conv2d(__TIME__), mm = conv2d(__TIME__ + 3), ss = conv2d(__TIME__ + 6); // Get H, M, S from compile time

boolean initial = true;

void setup(void) {
    delay(1000); // Wait for the display to power up

    tft.begin(); // Initialize the display
    delay(500); // Wait for 500ms to ensure the display is ready

    tft.setRotation(0);
    tft.fillScreen(TFT_CYBERPUNK_BG);
    tft.setTextColor(TFT_CYBERPUNK_TEXT, TFT_CYBERPUNK_BG); // Adding a background colour erases previous text automatically

    drawClockFace();
    targetTime = millis() + 1000;
}

void loop() {
    if (targetTime < millis()) {
        targetTime += 1000;
        updateClock();
    }
}

void drawClockFace() {
    tft.fillCircle(120, 120, 118, TFT_CYBERPUNK_MARKER);
    tft.fillCircle(120, 120, 110, TFT_CYBERPUNK_BG);

    // Draw 12 lines
    for (int i = 0; i < 360; i += 30) {
        drawLine(i, 114, 100, TFT_CYBERPUNK_MARKER);
    }

    // Draw 60 dots
    for (int i = 0; i < 360; i += 6) {
        drawDot(i, 102, TFT_CYBERPUNK_TEXT);
    }

    tft.fillCircle(120, 121, 3, TFT_CYBERPUNK_TEXT);
    tft.drawCentreString("Time flies", 120, 240, 4);
}

void drawLine(int angle, int radius1, int radius2, uint16_t color) {
    float rad = (angle - 90) * 0.0174532925;
    sx = cos(rad);
    sy = sin(rad);
    int x0 = sx * radius1 + 120;
    int yy0 = sy * radius1 + 120;
    int x1 = sx * radius2 + 120;
    int yy1 = sy * radius2 + 120;
    tft.drawLine(x0, yy0, x1, yy1, color);
}

void drawDot(int angle, int radius, uint16_t color) {
    float rad = (angle - 90) * 0.0174532925;
    sx = cos(rad);
    sy = sin(rad);
    int x0 = sx * radius + 120;
    int yy0 = sy * radius + 120;
    tft.drawPixel(x0, yy0, color);

    if (angle == 0 || angle == 180 || angle == 90 || angle == 270) {
        tft.fillCircle(x0, yy0, 2, color);
    }
}

void updateClock() {
    ss++;
    if (ss == 60) {
        ss = 0;
        mm++;
        if (mm == 60) {
            mm = 0;
            hh++;
            if (hh == 24) {
                hh = 0;
            }
        }
    }

    sdeg = ss * 6;
    mdeg = mm * 6 + sdeg * 0.01666667;
    hdeg = hh * 30 + mdeg * 0.0833333;
    hx = cos((hdeg - 90) * 0.0174532925);
    hy = sin((hdeg - 90) * 0.0174532925);
    mx = cos((mdeg - 90) * 0.0174532925);
    my = sin((mdeg - 90) * 0.0174532925);
    sx = cos((sdeg - 90) * 0.0174532925);
    sy = sin((sdeg - 90) * 0.0174532925);

    if (ss == 0 || initial) {
        initial = false;
        tft.drawLine(ohx, ohy, 120, 121, TFT_CYBERPUNK_BG);
        ohx = hx * 62 + 121;
        ohy = hy * 62 + 121;
        tft.drawLine(omx, omy, 120, 121, TFT_CYBERPUNK_BG);
        omx = mx * 84 + 120;
        omy = my * 84 + 121;
    }

    tft.drawLine(osx, osy, 120, 121, TFT_CYBERPUNK_BG);
    osx = sx * 90 + 121;
    osy = sy * 90 + 121;
    tft.drawLine(osx, osy, 120, 121, TFT_CYBERPUNK_SECOND);
    tft.drawLine(ohx, ohy, 120, 121, TFT_CYBERPUNK_HOUR);
    tft.drawLine(omx, omy, 120, 121, TFT_CYBERPUNK_MINUTE);
    tft.drawLine(osx, osy, 120, 121, TFT_CYBERPUNK_SECOND);

    tft.fillCircle(120, 121, 3, TFT_CYBERPUNK_SECOND);
}

static uint8_t conv2d(const char* p) {
    uint8_t v = 0;
    if ('0' <= *p && *p <= '9')
        v = *p - '0';
    return 10 * v + *++p - '0';
}
