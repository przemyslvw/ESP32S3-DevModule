#include <Arduino.h>
#include <TinyUSB.h>

// Symulacja klawiatury
Adafruit_USBD_HID usb_hid;
uint8_t const desc_hid_report[] = {
  TUD_HID_REPORT_DESC_KEYBOARD()    // Raport klawiatury
};

// Tablica mapująca klawisze
enum {
  KEY_CTRL  = 0xE0,
  KEY_SHIFT = 0xE1,
  KEY_ALT   = 0xE2,
  KEY_GUI   = 0xE3,
  KEY_ENTER = 0x28,
  KEY_C     = 0x06,
  KEY_W     = 0x1A,
  KEY_D     = 0x07,
  KEY_L     = 0x0F,
  KEY_ENTER = 0x28
};

// Funkcja wysyłająca dane klawiatury
void sendKey(uint8_t key) {
  uint8_t keycode[6] = {0};  // Domyślnie brak klawiszy
  keycode[0] = key;          // Ustawienie klawisza do wysłania
  usb_hid.keyboardReport(0, 0, keycode); // Wysłanie raportu HID
  delay(100);               // Krótkie opóźnienie
  usb_hid.keyboardRelease(); // Zwolnienie klawisza
}

void setup() {
  Serial.begin(115200);
  
  // Inicjalizacja TinyUSB jako urządzenia HID
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.begin();
  
  // Czekamy na połączenie USB
  while( !TinyUSBDevice.mounted() ) delay(1);

  // Poczekaj kilka sekund, by upewnić się, że komputer gotowy
  delay(3000);

  // Otwarcie przeglądarki: WIN + R
  usb_hid.keyboardPress(KEY_GUI);  // Wciśnięcie klawisza Windows (GUI)
  sendKey('r');  // Wciśnięcie "r" w celu otwarcia okna dialogowego "Uruchom"

  delay(500);  // Czekamy na otwarcie okna "Uruchom"

  // Wpisanie komendy do otwarcia Chrome
  sendKey('c');
  sendKey('h');
  sendKey('r');
  sendKey('o');
  sendKey('m');
  sendKey('e');
  sendKey(KEY_ENTER);  // Wciśnięcie Enter, aby uruchomić Chrome

  delay(2000);  // Poczekaj na uruchomienie przeglądarki

  // Wpisanie linku do YouTube
  sendKey('w');
  sendKey('w');
  sendKey('w');
  sendKey('.');
  sendKey('y');
  sendKey('o');
  sendKey('u');
  sendKey('t');
  sendKey('u');
  sendKey('b');
  sendKey('e');
  sendKey('.');
  sendKey('c');
  sendKey('o');
  sendKey('m');
  sendKey(KEY_ENTER);  // Wciśnięcie Enter

  delay(1000);  // Poczekaj, aż strona się załaduje
}

void loop() {
  // Nic do zrobienia w pętli głównej
}