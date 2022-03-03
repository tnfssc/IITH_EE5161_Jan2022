
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncUDP.h>
#include <esp32PWMUtilities.h>

const char *ssid = "tnfsscph1";
const char *password = "22222222";

AsyncUDP udp;

Motor Motor1;
Motor Motor2;

// Mutex lock for the motor to avoid race conditions
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

void setup()
{
  Motor1.attach(14, 16, 17);
  Motor2.attach(15, 18, 19);

  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.println("Connecting to WiFi...");
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("WiFi failed");
    return;
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Start the UDP server on port 8000
  if (udp.listen(8000))
  {
    Serial.println("UDP Listening");
    // This function is called when any data is received
    udp.onPacket([](AsyncUDPPacket packet)
                 {
      // Taking the first 4 characters of the received data and converting to a string
      String str = (char *)packet.data();
      str = str.substring(0, 4);
      if (str.compareTo("forw") == 0)
      {
        moveForward();
      }
      else if (str.compareTo("back") == 0)
      {
        moveBackward();
      }
      else if (str.compareTo("righ") == 0)
      {
        moveRight();
      }
      else if (str.compareTo("left") == 0)
      {
        moveLeft();
      }
      else
      {
        stop();
      }
      Serial.println(str); });
  }
  else
  {
    Serial.println("UDP Listen failed");
  }
}

void loop()
{
}