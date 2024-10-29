#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <DHT.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Ustawienia czujników
#define DHTPIN 15
#define DHTTYPE DHT22
#define LED_PIN 27
#define LED_COUNT 10

DHT dht(DHTPIN, DHTTYPE);
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Ustawienia BLE
BLEServer *pServer = NULL;
BLECharacteristic *pTempCharacteristic = NULL;
BLECharacteristic *pHeartCharacteristic = NULL;
BLECharacteristic *pLEDCharacteristic = NULL;
bool deviceConnected = false;

// UUID dla BLE
#define SERVICE_UUID           "12345678-1234-1234-1234-1234567890ab"
#define TEMP_CHARACTERISTIC_UUID "abcd1234-5678-90ab-cdef-1234567890ab"
#define HEART_CHARACTERISTIC_UUID "abcd5678-1234-90ab-cdef-1234567890ab"
#define LED_CHARACTERISTIC_UUID   "abcd90ab-1234-5678-cdef-1234567890ab"

// Zmienne do pomiarów
int heartRate = 0;
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

class LEDCallbacks: public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string value = pCharacteristic->getValue();
    if (value.length() == 3) {
      int r = value[0];
      int g = value[1];
      int b = value[2];
      for(int i = 0; i < LED_COUNT; i++) {
        strip.setPixelColor(i, strip.Color(r, g, b));
      }
      strip.show();
    }
  }
};

void setup() {
  Serial.begin(115200);
  dht.begin();
  strip.begin();
  strip.show();

  // Inicjalizacja BLE
  BLEDevice::init("ESP32S3_HealthMonitor");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Charakterystyka temperatury
  pTempCharacteristic = pService->createCharacteristic(
                        TEMP_CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
                      );
  pTempCharacteristic->addDescriptor(new BLE2902());

  // Charakterystyka tętna
  pHeartCharacteristic = pService->createCharacteristic(
                         HEART_CHARACTERISTIC_UUID,
                         BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
                       );
  pHeartCharacteristic->addDescriptor(new BLE2902());

  // Charakterystyka LED
  pLEDCharacteristic = pService->createCharacteristic(
                        LED_CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_WRITE
                      );
  pLEDCharacteristic->setCallbacks(new LEDCallbacks());

  pService->start();
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();
}

void loop() {
  if (deviceConnected) {
    // Odczyt temperatury i wilgotności
    temperature = dht.readTemperature();
    humidity = dht.readHumidity();
    if (!isnan(temperature) && !isnan(humidity)) {
      String tempData = String(temperature) + "C, " + String(humidity) + "%";
      pTempCharacteristic->setValue(tempData.c_str());
      pTempCharacteristic->notify();
    }

    // Symulacja odczytu tętna (dla uproszczenia)
    heartRate = random(60, 100);
    String heartData = "HR: " + String(heartRate) + " bpm";
    pHeartCharacteristic->setValue(heartData.c_str());
    pHeartCharacteristic->notify();

    // Pauza co 2 sekundy między odczytami
    delay(2000);
  }
}