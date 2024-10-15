#include <Arduino.h>
#include <TinyUSB.h>

// Symulacja klawiatury
Adafruit_USBD_HID usb_hid;
uint8_t const desc_hid_report[] = {
  TUD_HID_REPORT_DESC_KEYBOARD()    // Raport klawiatury
};

// Tablica mapująca klawisze
enum {
  KEY_A = 0x04,
  KEY_B = 0x05,
  KEY_C = 0x06,
  KEY_D = 0x07,
  KEY_E = 0x08,
  KEY_F = 0x09,
  KEY_G = 0x0A,
  KEY_H = 0x0B,
  KEY_I = 0x0C,
  KEY_J = 0x0D,
  KEY_K = 0x0E,
  KEY_L = 0x0F,
  KEY_M = 0x10,
  KEY_N = 0x11,
  KEY_O = 0x12,
  KEY_P = 0x13,
  KEY_Q = 0x14,
  KEY_R = 0x15,
  KEY_S = 0x16,
  KEY_T = 0x17,
  KEY_U = 0x18,
  KEY_V = 0x19,
  KEY_W = 0x1A,
  KEY_X = 0x1B,
  KEY_Y = 0x1C,
  KEY_Z = 0x1D,
  KEY_1 = 0x1E,
  KEY_2 = 0x1F,
  KEY_3 = 0x20,
  KEY_4 = 0x21,
  KEY_5 = 0x22,
  KEY_6 = 0x23,
  KEY_7 = 0x24,
  KEY_8 = 0x25,
  KEY_9 = 0x26,
  KEY_0 = 0x27,
  KEY_ENTER = 0x28,
  KEY_ESC = 0x29,
  KEY_BACKSPACE = 0x2A,
  KEY_TAB = 0x2B,
  KEY_SPACE = 0x2C,
  KEY_MINUS = 0x2D,
  KEY_EQUAL = 0x2E,
  KEY_LEFT_BRACE = 0x2F,
  KEY_RIGHT_BRACE = 0x30,
  KEY_BACKSLASH = 0x31,
  KEY_SEMICOLON = 0x33,
  KEY_APOSTROPHE = 0x34,
  KEY_GRAVE = 0x35,
  KEY_COMMA = 0x36,
  KEY_PERIOD = 0x37,
  KEY_SLASH = 0x38,
  KEY_CAPS_LOCK = 0x39,
  KEY_F1 = 0x3A,
  KEY_F2 = 0x3B,
  KEY_F3 = 0x3C,
  KEY_F4 = 0x3D,
  KEY_F5 = 0x3E,
  KEY_F6_MINUS = 0x56,
  KEYPAD_PLUS = 0x57,
  KEYPAD_ENTER = 0x58,
  KEYPAD_1 = 0x59,
  KEYPAD_2 = 0x5A,
  KEYPAD_3 = 0x5B,
  KEYPAD_4 = 0x5C,
  KEYPAD_5 = 0x5D,
  KEYPAD_6 = 0x5E,
  KEYPAD_7 = 0x5F,
  KEYPAD_8 = 0x60,
  KEYPAD_9 = 0x61,
  KEYPAD_0 = 0x62,
  KEYPAD_PERIOD = 0x63,
  KEY_CTRL = 0xE0,
  KEY_SHIFT = 0xE1,
  KEY_ALT = 0xE2,
  KEY_GUI = 0xE3
};

// Funkcja konwertująca znaki na kody klawiszy
uint8_t charToKeycode(char c) {
  if (c >= 'a' && c <= 'z') {
    return KEY_A + (c - 'a');
  } else if (c >= 'A' && c <= 'Z') {
    return KEY_A + (c - 'A');
  } else if (c >= '0' && c <= '9') {
    return KEY_1 + (c - '1');
  }
  // Dodaj więcej warunków dla innych znaków
  return 0;
}

// Funkcja wysyłająca dane klawiatury
void sendKey(uint8_t key) {
  uint8_t keycode[6] = {0};  // Domyślnie brak klawiszy
  keycode[0] = key;          // Ustawienie klawisza do wysłania
  usb_hid.keyboardReport(0, 0, keycode); // Wysłanie raportu HID
  delay(100);               // Krótkie opóźnienie
  usb_hid.keyboardRelease(); // Zwolnienie klawisza
}

void sendChar(char c) {
  sendKey(charToKeycode(c));
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
  usb_hid.keyboardPress(KEY_GUI);  // Wciśnięcie klawisza Windows (GUIamy na otwarcie okna "Uruchom"

  // Wpisanie komendy do otwarcia Chrome
  sendChar('c');
  sendChar('h');
  sendChar('r');
  sendChar('o');
  sendChar('m');
  sendChar('e');
  sendKey(KEY_ENTER);  // Wciśnięcie Enter, aby uruchomić Chrome

  delay(2000);  // Poczekaj na uruchomienie przeglądarki

  // Wpisanie linku do YouTube
  sendChar('w');
  sendChar('w');
  sendChar('w');
  sendChar('.');
  sendChar('y');
  sendChar('o');
  sendChar('u');
  sendChar('t');
  sendChar('u');
  sendChar('b');
  sendChar('e');
  sendChar('.');
  sendChar('c');
  sendChar('o');
  sendChar('m');
  sendKey(KEY_ENTER);  // Wciśnięcie Enter

  delay(1000);  // Poczekaj, aż strona się załaduje
}

void loop() {
  // Nic do zrobienia w pętli głównej
}