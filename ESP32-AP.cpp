#include <WiFi.h>
#include <WebServer.h>

// Ustawienia nazwy sieci (SSID) i hasła (opcjonalne)
const char* ssid = "ESP32-AP";
const char* password = "12345678";

// Tworzymy serwer WWW na porcie 80
WebServer server(80);

// Funkcja odpowiedzialna za wyświetlanie strony głównej
void handleRoot() {
  String html = "<!DOCTYPE html><html>";
  html += "<head><title>ESP32 WiFi Hotspot</title></head>";
  html += "<body><h1>Witaj na ESP32!</h1>";
  html += "<p>Jesteś połączony z hotspotem ESP32.</p>";
  html += "</body></html>";

  // Odpowiedź HTTP
  server.send(200, "text/html", html);
}

void setup() {
  // Inicjalizacja portu szeregowego (do debugowania)
  Serial.begin(115200);
  delay(1000);

  // Tryb Access Point (hotspot)
  WiFi.softAP(ssid, password);

  // Wyświetlanie informacji o lokalnym IP
  Serial.println();
  Serial.print("Hotspot uruchomiony. Adres IP: ");
  Serial.println(WiFi.softAPIP());

  // Ustawienie strony głównej
  server.on("/", handleRoot);

  // Uruchomienie serwera HTTP
  server.begin();
  Serial.println("Serwer HTTP uruchomiony.");
}

void loop() {
  // Obsługa klientów HTTP
  server.handleClient();
}
