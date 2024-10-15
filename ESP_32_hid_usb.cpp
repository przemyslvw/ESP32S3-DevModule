#include <Arduino.h>
#include <TinyUSB.h>

// Symulacja klawiatury
Adafruit_USBD_HID usb_hid;
uint8_t const desc_hid_report[] = {
  TUD_HID_REPORT_DESC_KEYBOARD()    // Raport klawiatury
};

// Tablica mapująca klawisze
enum KeyCodes {
  KEY_A = 0x04, KEY_B, KEY_C, KEY_D, KEY_E, KEY_F, KEY_G, KEY_H, KEY_I, KEY_J,
  KEY_K, KEY_L, KEY_M, KEY_N, KEY_O, KEY_P, KEY_Q, KEY_R, KEY_S, KEY_T,
  KEY_U, KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
  KEY_1 = 0x1E, KEY_2, KEY_3, KEY_4, KEY_5, KEY_6, KEY_7, KEY_8, KEY_9, KEY_0,
  KEY_ENTER = 0x28, KEY_ESC, KEY_BACKSPACE, KEY_TAB, KEY_SPACE, KEY_MINUS,
  KEY_EQUAL, KEY_LEFT_BRACE, KEY_RIGHT_BRACE, KEY_BACKSLASH,
  KEY_SEMICOLON = 0x33, KEY_APOSTROPHE, KEY_GRAVE, KEY_COMMA, KEY_PERIOD, KEY_SLASH,
  KEY_CAPS_LOCK = 0x39, KEY_F1, KEY_F2, KEY_F3, KEY_F4, KEY_F5, KEY_F6_MINUS = 0x56, 
  KEYPAD_PLUS, KEYPAD_ENTER, KEYPAD_1, KEYPAD_2, KEYPAD_3, KEYPAD_4, 
  KEYPAD_5, KEYPAD_6, KEYPAD_7, KEYPAD_8, KEYPAD_9, KEYPAD_0, KEYPAD_PERIOD,
  KEY_CTRL = 0xE0, KEY_SHIFT, KEY_ALT, KEY_GUI
};

// Funkcja konwertująca znaki na kody klawiszy
uint8_t charToKeycode(char c) {
  if (c >= 'a' && c <= 'z')
    return KEY_A + (c - 'a');
  else if (c >= 'A' && c <= 'Z')
    return KEY_A + (c - 'A');
  else if (c >= '0' && c <= '9')
    return KEY_1 + (c - '1');
  // Dodaj więcej warunków dla innych znaków
  return 0;
}

// Funkcja wysyłająca dane klawiatury
void sendKey(uint8_t key) {
  uint8_t keycode[6] = {key};
  usb_hid.keyboardReport(0, 0, keycode);
  delay(100);
  usb_hid.keyboardRelease();
}

void sendChar(char c) {
  uint8_t keycode = charToKeycode(c);
  if (keycode) sendKey(keycode);
}

void initializeHID() {
  usb_hid.setPollInterval(2);
  usb_hid.setReportDescriptor(desc_hid_report, sizeof(desc_hid_report));
  usb_hid.begin();
  while (!TinyUSBDevice.mounted()) delay(1);
}

void openChrome() {
  // Otwarcie przeglądarki: WIN + R
  usb_hid.keyboardPress(KEY_GUI);
  sendChar('r');
  delay(500);

  // Wpisanie komendy do otwarcia Chrome
  const char* chromeCmd = "chrome";
  while (*chromeCmd) sendChar(*chromeCmd++);

  sendKey(KEY_ENTER);
  delay(2000);
}

void openYouTube() {
  // Wpisanie linku do YouTube
  const char* youtubeURL = "www.youtube.com";
  while (*youtubeURL) sendChar(*youtubeURL++);

  sendKey(KEY_ENTER);
  delay(1000);
}

void setup() {
  Serial.begin(115200);
  initializeHID();
  delay(3000);  // Poczekaj kilka sekund, by upewnić się, że komputer gotowy

  openChrome();
  openYouTube();
}

void loop() {
  // Nic do zrobienia w pętli głównej
}