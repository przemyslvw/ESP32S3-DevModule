#include <TFT_eSPI.h>
#include <SPI.h>

// Inicjalizacja obiektu TFT
TFT_eSPI tft = TFT_eSPI(); 

// Proste parametry animacji
int x = 10;
int y = 10;
int xSpeed = 3;
int ySpeed = 3;
int radius = 10;

void setup() {
  // Inicjalizacja wyświetlacza
  tft.init();
  tft.setRotation(1); // Ustawienie orientacji wyświetlacza
  tft.fillScreen(TFT_BLACK); // Wypełnienie ekranu na czarno
}

void loop() {
  // Czyszczenie poprzedniego stanu
  tft.fillCircle(x, y, radius, TFT_BLACK);
  
  // Aktualizacja pozycji
  x += xSpeed;
  y += ySpeed;
  
  // Odbicie od krawędzi ekranu
  if (x - radius <= 0 || x + radius >= tft.width()) {
    xSpeed = -xSpeed;
  }
  if (y - radius <= 0 || y + radius >= tft.height()) {
    ySpeed = -ySpeed;
  }
  
  // Rysowanie nowego stanu
  tft.fillCircle(x, y, radius, TFT_GREEN);
  
  // Krótkie opóźnienie dla animacji
  delay(30);
}
