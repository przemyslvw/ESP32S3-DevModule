#include <Wire.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <DHT.h>

// Ustawienia czujnika DHT
#define DHTPIN 15
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// Ustawienia przekaźnika (lub diody LED) do sterowania wentylatorem
#define RELAY_PIN 14

// Ustawienia BLE
BLEServer *pServer = NULL;
BLECharacteristic *pTempCharacteristic = NULL;
BLECharacteristic *pRelayCharacteristic = NULL;
bool deviceConnected = false;

// UUID dla BLE
#define SERVICE_UUID               "abcd1234-5678-90ab-cdef-1234567890ab"
#define TEMP_CHARACTERISTIC_UUID   "temp1234-5678-90ab-cdef-1234567890ab"
#define RELAY_CHARACTERISTIC_UUID  "relay5678-1234-abcd-5678-abcdef123456"

// Zmienne do przechowywania temperatury i wilgotności
float temperature = 0.0;
float humidity = 0.0;

// Callbacki BLE
class ServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };
  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

class RelayCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    if (value.length() > 0) {
      // Odczyt wartości i sterowanie przekaźnikiem (0 = wyłącz, 1 = włącz)
      if (value[0] == '1') {
        digitalWrite(RELAY_PIN, HIGH);
      } else {
        digitalWrite(RELAY_PIN, LOW);
      }
    }
  }
};

void setup() {
  Serial.begin(115200);
  dht.begin();

  // Inicjalizacja przekaźnika
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Startowo wyłączony

  // Inicjalizacja BLE
  BLEDevice::init("ESP32S3_EnvMonitor");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Charakterystyka temperatury i wilgotności
  pTempCharacteristic = pService->createCharacteristic(
                         TEMP_CHARACTERISTIC_UUID,
                         BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
                       );
  pTempCharacteristic->addDescriptor(new BLE2902());

  // Charakterystyka przekaźnika
  pRelayCharacteristic = pService->createCharacteristic(
                          RELAY_CHARACTERISTIC_UUID,
                          BLECharacteristic::PROPERTY_WRITE
                        );
  pRelayCharacteristic->setCallbacks(new RelayCallbacks());

  pService->start();

  // Beacon BLE
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->start();
}

void loop() {
  if (deviceConnected) {
    // Odczyt temperatury i wilgotności
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    if (!isnan(temperature) && !isnan(humidity)) {
      // Wysyłanie danych do aplikacji przez BLE
      String tempData = "Temp: " + String(temperature) + "C, Humidity: " + String(humidity) + "%";
      pTempCharacteristic->setValue(tempData.c_str());
      pTempCharacteristic->notify();
    }

    // Pauza co 2 sekundy między odczytami
    delay(2000);
  }
}