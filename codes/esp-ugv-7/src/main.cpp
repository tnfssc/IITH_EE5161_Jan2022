
#include <Arduino.h>
#include <WiFi.h>
#include <AsyncUDP.h>
#include <esp32PWMUtilities.h>

const char *ssid = "beingHuman";
const char *password = "1234qwer!@#$QWER";

AsyncUDP udp;

Motor Motor1;
Motor Motor2;

void stop()
{
  delay(250);
  Motor1.lockMotor();
  Motor2.lockMotor();
}
void moveForward()
{
  Motor1.moveMotor(2.55 * 100);
  Motor2.moveMotor(2.55 * 100);
  stop();
}
void moveBackward()
{
  Motor1.moveMotor(-2.55 * 100);
  Motor2.moveMotor(-2.55 * 100);
  stop();
}
void moveRight()
{
  Motor1.moveMotor(2.55 * 100);
  Motor2.moveMotor(-2.55 * 100);
  stop();
}
void moveLeft()
{
  Motor1.moveMotor(-2.55 * 100);
  Motor2.moveMotor(2.55 * 100);
  stop();
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

  if (udp.listen(8000))
  {
    Serial.println("UDP Listening");
    udp.onPacket([](AsyncUDPPacket packet)
                 {
      Serial.print("UDP Packet Type: ");
      Serial.write(packet.data(), packet.length()); });
  }
  else
  {
    Serial.println("UDP Listen failed");
  }
}

void loop()
{
}