#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <ESP32Servo.h>

// Modified from https://randomnerdtutorials.com/esp32-bluetooth-low-energy-ble-arduino-ide/

Servo rightLeg;
Servo leftLeg;
Servo dome;

#define SERVICE_ID "f558b227-852b-4845-b327-f0f56d153fc6"
#define DRIVE_ID "60fbcc76-8214-4713-9468-8f6d732a0e38"
#define DOME_ID "325a5884-779e-4430-9c43-a11f541e73fb"

class DebugCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *characteristic)
  {
    std::string value = characteristic->getValue();

    if (value.length() > 0)
    {
      Serial.println("*********");
      Serial.print("New value: ");
      for (int i = 0; i < value.length(); i++)
        Serial.print(value[i]);

      Serial.println();
      Serial.println("*********");
    }
  }
};

void setup()
{
  Serial.begin(115200);

  Serial.println("Starting BLE Server...");
  BLEDevice::init("R2-D2");
  BLEServer *server = BLEDevice::createServer();
  BLEService *service = server->createService(SERVICE_ID);

  BLECharacteristic *driveCharacteristic = service->createCharacteristic(
      DRIVE_ID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
  driveCharacteristic->setValue("STOP");
  driveCharacteristic->setCallbacks(new DebugCallbacks());
  BLECharacteristic *domeCharacteristic = service->createCharacteristic(
      DOME_ID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE | BLECharacteristic::PROPERTY_NOTIFY);
  domeCharacteristic->setValue("STOP");
  domeCharacteristic->setCallbacks(new DebugCallbacks());
  service->start();

  BLEAdvertising *advertising = BLEDevice::getAdvertising();
  advertising->addServiceUUID(SERVICE_ID);
  advertising->setScanResponse(true);
  advertising->setMinPreferred(0x06);
  advertising->setMinPreferred(0x12);
  BLEDevice::startAdvertising();
  Serial.println("BLE Server setup complete.");

  Serial.println("Attaching servos...");
  rightLeg.attach(4);
  leftLeg.attach(16);
  dome.attach(17);
  Serial.println("Servos attached.");

  Serial.println("R2-D2 reporting for duty!");
}

void loop()
{
  // rightLeg.write(180);
  // leftLeg.write(180);
  // dome.write(180);
  // delay(5000);
  // rightLeg.write(0);
  // leftLeg.write(0);
  // dome.write(0);
  // delay(5000);
  // rightLeg.write(90);
  // leftLeg.write(90);
  // dome.write(90);
  // delay(5000);
}