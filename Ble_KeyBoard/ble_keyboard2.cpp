#include <WiFi.h>
#include <WebServer.h>

WebServer server(80);

const char* ssid = "TwojaSiec";
const char* password = "TwojeHaslo";

void handleRoot() {
  server.send(200, "text/html", "<h1>ESP32 BLE Keyboard</h1><br>"
                                "<input type='text' id='input' oninput='sendKey(this.value.charCodeAt(this.value.length-1))'>"
                                "<script>"
                                "function sendKey(charCode) {"
                                "  fetch('/sendKey?key=' + charCode);"
                                "}"
                                "</script>");
}

void handleSendKey() {
  String key = server.arg("key");
  if (key.length() > 0) {
    uint8_t keycode = key.toInt();
    sendKey(keycode);  // Wywołanie funkcji BLE do wysyłania kluczy
  }
  server.send(200, "text/plain", "OK");
}

void setup() {
  Serial.begin(115200);
  
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Łączenie z siecią Wi-Fi...");
  }
  Serial.println("Połączono!");

  server.on("/", handleRoot);
  server.on("/sendKey", handleSendKey);
  
  server.begin();

  xTaskCreate(bleKeyboardTask, "BLE Keyboard Task", 5000, NULL, 1, NULL);
}

void loop() {
  server.handleClient();
}