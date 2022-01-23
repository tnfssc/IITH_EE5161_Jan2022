#include <Arduino.h>

#include <WiFi.h>
#include <esp32PWMUtilities.h>

#include <DabbleESP32.h>
Motor Motor1;

Motor Motor2;
void _setup()
{
  Motor1.attach(14, 16, 17);

  Motor2.attach(15, 18, 19);

  Dabble.begin("MyEsp32");
}

void _loop()
{
  Dabble.processInput();
}

void setup()
{
  _setup();
}

void turnRight()
{
  Motor1.moveMotor(2.55 * 100);
  Motor2.moveMotor(-2.55 * 100);
  delay(500);
}

void turnLeft()
{
  Motor1.moveMotor(-2.55 * 100);
  Motor2.moveMotor(2.55 * 100);
  delay(500);
}

void goForward(int time)
{
  Motor1.moveMotor(2.55 * 100);
  Motor2.moveMotor(2.55 * 100);
  delay(time);
}

void traceRect(int size)
{
  goForward(size);
  turnRight();
  goForward(size);
  turnRight();
  goForward(size);
  turnRight();
  goForward(size);
  turnRight();
}

void loop()
{
  _loop();
  if (GamePad.isPressed(0))
  {
    for (int i = 10; i >= 0; i--)
    {
      traceRect(2000);
    }
  }
  else if (GamePad.isPressed(3))
  {
    turnRight();
  }
  else
  {
    Motor1.lockMotor();
    Motor2.lockMotor();
  }
}
