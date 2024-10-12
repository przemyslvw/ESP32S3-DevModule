#include <BLEDevice.h>
#include <BLEHIDDevice.h>
#include <HIDTypes.h>
#include <HIDKeyboardTypes.h>
#include <BLEServer.h>
#include <BLE2902.h>

BLEHIDDevice* hid;
BLECharacteristic* input;
BLECharacteristic* output;
BLEAdvertising* advertising;

void bleKeyboardTask(void*) {
  BLEDevice::init("ESP32 BLE Keyboard");

  BLEServer* server = BLEDevice::createServer();
  hid = new BLEHIDDevice(server);

  input = hid->inputReport(1);  // Input report
  output = hid->outputReport(1); // Output report

  // Setting up HID info
  hid->manufacturer()->setValue("ESP32");
  hid->pnp(0x02, 0xe502, 0xa111, 0x0210);
  hid->hidInfo(0x00, 0x01);

  const uint8_t reportMap[] = {
      USAGE_PAGE(1), 0x01,       // Generic Desktop
      USAGE(1), 0x06,            // Keyboard
      COLLECTION(1), 0x01,       // Application
      REPORT_ID(1), 0x01,        // Report ID 1
      USAGE_PAGE(1), 0x07,       // Keyboard/Keypad
      USAGE_MINIMUM(1), 0xE0,    // Keyboard Left Control
      USAGE_MAXIMUM(1), 0xE7,    // Keyboard Right GUI
      LOGICAL_MINIMUM(1), 0x00,
      LOGICAL_MAXIMUM(1), 0x01,
      REPORT_SIZE(1), 0x01,
      REPORT_COUNT(1), 0x08,
      INPUT(1), 0x02,            // Data, Var, Abs
      REPORT_COUNT(1), 0x01,
      REPORT_SIZE(1), 0x08,
      INPUT(1), 0x01,            // Const, Array, Abs
      REPORT_COUNT(1), 0x05,
      REPORT_SIZE(1), 0x01,
      USAGE_PAGE(1), 0x08,       // LEDs
      USAGE_MINIMUM(1), 0x01,    // Num Lock
      USAGE_MAXIMUM(1), 0x05,    // Kana
      OUTPUT(1), 0x02,           // Data, Var, Abs
      REPORT_COUNT(1), 0x01,
      REPORT_SIZE(1), 0x03,
      OUTPUT(1), 0x01,           // Const, Array, Abs
      END_COLLECTION(0)
  };

  hid->reportMap((uint8_t*)reportMap, sizeof(reportMap));
  hid->startServices();

  advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(hid->hidService()->getUUID());
  advertising->setScanResponse(true);
  advertising->start();
  
  hid->setBatteryLevel(100);
  
  vTaskDelete(NULL);
}

void sendKey(uint8_t keycode) {
  uint8_t report[] = { 0x00, 0x00, keycode, 0x00, 0x00, 0x00, 0x00, 0x00 };
  input->setValue(report, sizeof(report));
  input->notify();
  delay(50);

  // Release the key
  report[2] = 0;
  input->setValue(report, sizeof(report));
  input->notify();
}