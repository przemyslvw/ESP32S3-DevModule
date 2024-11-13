#include <WiFi.h>
#include <PubSubClient.h>

// Definicje pinów dla diod RGB
#define RED_LED_PIN 4
#define GREEN_LED_PIN 16
#define BLUE_LED_PIN 17

// Ustawienia Wi-Fi
const char* ssid = "Twoja_Siec_WiFi";
const char* password = "Twoje_Haslo_WiFi";

// Ustawienia MQTT
const char* mqtt_server = "192.168.0.12"; // Adres IP Raspberry Pi
const int mqtt_port = 1883;               // Domyślny port MQTT
const char* mqtt_topic = "home/alerts";   // Temat MQTT dla alertów

WiFiClient espClient;
PubSubClient client(espClient);

// Funkcja konfigurująca WiFi
void setup_wifi() {
  delay(10);
  Serial.println("Łączenie z WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Połączono z WiFi");
}

// Funkcja obsługująca przychodzące wiadomości MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  String message;
  for (int i = 0; i < length; i++) {
    message += (char)payload[i];
  }
  Serial.print("Otrzymano wiadomość: ");
  Serial.println(message);

  // Ustawienie koloru diody na podstawie otrzymanej wiadomości
  if (message == "OK") {
    setRGB(0, 255, 0);    // Zielony - OK
  } else if (message == "WARNING") {
    setRGB(255, 255, 0);  // Żółty - Ostrzeżenie
  } else if (message == "ERROR") {
    setRGB(255, 0, 0);    // Czerwony - Błąd
  } else if (message == "INFO") {
    setRGB(0, 0, 255);    // Niebieski - Informacja
  }
}

// Funkcja inicjalizująca połączenie z brokerem MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Łączenie z MQTT...");
    if (client.connect("ESP32Client")) {
      Serial.println("Połączono z MQTT");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("Błąd połączenia z MQTT, kod błędu: ");
      Serial.print(client.state());
      Serial.println(" Próba ponownego połączenia za 5 sekund.");
      delay(5000);
    }
  }
}

// Funkcja ustawiająca kolor diod RGB
void setRGB(int redValue, int greenValue, int blueValue) {
  analogWrite(RED_LED_PIN, redValue);
  analogWrite(GREEN_LED_PIN, greenValue);
  analogWrite(BLUE_LED_PIN, blueValue);
}

// Funkcja setup - konfiguracja początkowa
void setup() {
  Serial.begin(115200);

  pinMode(RED_LED_PIN, OUTPUT);
  pinMode(GREEN_LED_PIN, OUTPUT);
  pinMode(BLUE_LED_PIN, OUTPUT);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);
}

// Funkcja loop - główna pętla programu
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}