#include <WiFi.h>
#include <PubSubClient.h>

// Konfiguracja sieci Wi-Fi
const char* ssid = "Twoje_SSID";
const char* password = "Twoje_Haslo";

// Konfiguracja MQTT
const char* mqtt_server = "192.168.0.12"; // Adres Raspberry Pi
const int mqtt_port = 1883;
const char* mqtt_topic = "test/topic"; // Temat, z którego subskrybujemy dane

WiFiClient espClient;
PubSubClient client(espClient);
String mqttMessage = "Brak danych"; // Przechowywane dane z MQTT

// Funkcja obsługująca połączenie WiFi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Łączenie z ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Połączono z WiFi");
  Serial.print("Adres IP: ");
  Serial.println(WiFi.localIP());
}

// Funkcja wywoływana, gdy otrzymujemy wiadomość MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Wiadomość przychodząca z tematu: ");
  Serial.println(topic);

  mqttMessage = "";
  for (int i = 0; i < length; i++) {
    mqttMessage += (char)payload[i];
  }
  
  Serial.print("Otrzymane dane: ");
  Serial.println(mqttMessage);
}

// Funkcja do połączenia z brokerem MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Łączenie z MQTT...");
    if (client.connect("ESP32_Client")) {
      Serial.println("Połączono");
      client.subscribe(mqtt_topic);
    } else {
      Serial.print("Nieudana próba połączenia, kod: ");
      Serial.print(client.state());
      Serial.println(" Próbuję ponownie za 5 sekund...");
      delay(5000);
    }
  }
}

// Ustawienie serwera webowego
WiFiServer server(80);

void setup() {
  Serial.begin(115200);

  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // Start serwera webowego
  server.begin();
  Serial.println("Serwer webowy uruchomiony");
}

void loop() {
  // Obsługa połączenia MQTT
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Obsługa połączeń HTTP
  WiFiClient webClient = server.available();
  if (webClient) {
    Serial.println("Nowe połączenie do serwera webowego");

    // Odczytanie zapytania HTTP
    String request = webClient.readStringUntil('\r');
    Serial.println(request);
    webClient.flush();

    // Wysłanie nagłówków HTTP
    webClient.println("HTTP/1.1 200 OK");
    webClient.println("Content-type:text/html");
    webClient.println();

    // Wysłanie strony HTML z danymi MQTT
    webClient.println("<html><body><h1>Dane z MQTT</h1>");
    webClient.println("<p>Otrzymane dane: " + mqttMessage + "</p>");
    webClient.println("</body></html>");

    // Zamknięcie połączenia
    webClient.stop();
    Serial.println("Połączenie zakończone");
  }
}