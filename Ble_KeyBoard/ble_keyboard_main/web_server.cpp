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

void sendKey(uint8_t keycode); // Declaration of sendKey function

void handleSendKey() {
  const String key = server.arg("key");
  if (!key.isEmpty()) {
    const uint8_t keycode = key.toInt();
    sendKey(keycode);  // Wywołanie funkcji BLE do wysyłania kluczy
  }
  server.send(200, "text/plain", "OK");
}

void connectToWiFi() {
  WiFi.begin(ssid, password);
  Serial.print("Łączenie z siecią Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nPołączono!");
}
void bleKeyboardTask(void * parameter); // Declaration of bleKeyboardTask function

void setupWebServer() {
  connectToWiFi();

  server.on("/", handleRoot);
  server.on("/sendKey", handleSendKey);
  
  server.begin();
}

void handleWebServer() {
void loop() {
}oid loop() {
  server.handleClient();
}