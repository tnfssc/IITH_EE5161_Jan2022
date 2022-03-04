#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <esp32PWMUtilities.h>

#define SERVICE_UUID "1fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "aeb5483e-36e1-4688-b7f5-ea07361b26a8"

Motor Motor1;
Motor Motor2;

static bool lock = false;

void stop()
{
  Motor1.lockMotor();
  Motor2.lockMotor();
}
void moveForward()
{
  // If something is using the motors, don't touch it.
  if (lock)
    return;
  // Set lock to true to prevent others from using the motors
  lock = true;
  Motor1.moveMotor(2.55 * 100);
  Motor2.moveMotor(2.55 * 100);
  delay(250);
  stop();
  // Set lock to false to allow others to use the motors
  lock = false;
}
void moveBackward()
{
  if (lock)
    return;
  lock = true;
  Motor1.moveMotor(-2.55 * 100);
  Motor2.moveMotor(-2.55 * 100);
  delay(250);
  stop();
  lock = false;
}
void moveRight()
{
  if (lock)
    return;
  lock = true;
  Motor1.moveMotor(2.55 * 100);
  Motor2.moveMotor(-2.55 * 100);
  delay(250);
  stop();
  lock = false;
}
void moveLeft()
{
  if (lock)
    return;
  lock = true;
  Motor1.moveMotor(-2.55 * 100);
  Motor2.moveMotor(2.55 * 100);
  delay(250);
  stop();
  lock = false;
}

class MyCallbacks : public BLECharacteristicCallbacks
{
  void onWrite(BLECharacteristic *pCharacteristic)
  {
    std::string value = pCharacteristic->getValue();
    if (value.compare("forward") == 0)
      moveForward();
    else if (value.compare("backward") == 0)
      moveBackward();
    else if (value.compare("right") == 0)
      moveRight();
    else if (value.compare("left") == 0)
      moveLeft();
    else
      stop();
  }
};

void setup()
{
  Serial.begin(9600);
  Motor1.attach(14, 16, 17);
  Motor2.attach(15, 18, 19);
  BLEDevice::init("UGV");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
      CHARACTERISTIC_UUID,
      BLECharacteristic::PROPERTY_READ |
          BLECharacteristic::PROPERTY_WRITE);
  pCharacteristic->setCallbacks(new MyCallbacks());
  pCharacteristic->setValue("ESP is ready!");
  pService->start();
  BLEAdvertising *pAdvertising = pServer->getAdvertising();
  pAdvertising->start();
}

void loop()
{
  delay(2000);
}