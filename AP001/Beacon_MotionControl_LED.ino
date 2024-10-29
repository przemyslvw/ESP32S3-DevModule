#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include "MPU6050.h"

// Ustawienia LED
#define LED_PIN 27
#define LED_COUNT 10
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

// Ustawienia BLE
BLEServer *pServer = NULL;
BLECharacteristic *pAccCharacteristic = NULL;
BLECharacteristic *pLEDCharacteristic = NULL;
bool deviceConnected = false;

// UUID dla BLE
#define SERVICE_UUID               "a1b2c3d4-e5f6-1234-5678-90abcdef1234"
#define ACC_CHARACTERISTIC_UUID     "acc12345-6789-4321-bcda-1234567890ab"
#define LED_CHARACTERISTIC_UUID     "led56789-1234-abcd-5678-abcdef123456"

// Inicjalizacja MPU6050
MPU6050 mpu;
float accX, accY, accZ;

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
  Wire.begin();
  mpu.initialize();
  
  // Inicjalizacja LED
  strip.begin();
  strip.show();

  // Inicjalizacja BLE
  BLEDevice::init("ESP32S3_Beacon_Motion");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());

  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Charakterystyka akcelerometru
  pAccCharacteristic = pService->createCharacteristic(
                        ACC_CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
                      );
  pAccCharacteristic->addDescriptor(new BLE2902());

  // Charakterystyka LED
  pLEDCharacteristic = pService->createCharacteristic(
                        LED_CHARACTERISTIC_UUID,
                        BLECharacteristic::PROPERTY_WRITE
                      );
  pLEDCharacteristic->setCallbacks(new LEDCallbacks());

  pService->start();

  // Beacon BLE
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->start();
}

void loop() {
  if (deviceConnected) {
    // Odczyt danych akcelerometru
    accX = mpu.getAccelerationX();
    accY = mpu.getAccelerationY();
    accZ = mpu.getAccelerationZ();
    
    // Wysłanie danych przez BLE
    String accData = "X: " + String(accX) + " Y: " + String(accY) + " Z: " + String(accZ);
    pAccCharacteristic->setValue(accData.c_str());
    pAccCharacteristic->notify();

    // Pauza co 500 ms między odczytami
    delay(500);
  }
}