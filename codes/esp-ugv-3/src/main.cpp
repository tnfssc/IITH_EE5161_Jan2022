
#include <Arduino.h>
#include <SPIFFS.h>
#ifdef ESP32
#include <WiFi.h>
#include <AsyncTCP.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>
#include <esp32PWMUtilities.h>
#include <bits/stdc++.h>

AsyncWebServer server(80);

const char *ssid = "nitro5";
const char *password = "12345678";

const char *PARAM_MESSAGE = "message";

void notFound(AsyncWebServerRequest *request)
{
  request->send(404, "text/plain", "Not found");
}

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
  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("WiFi failed");
    return;
  }

  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // UI server
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/webui/index.html", "text/html"); });

  // Directions
  server.on("/forward", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              moveForward();
              request->send(200, "text/plain", "Done!");
            });

  server.on("/backward", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              moveBackward();
              request->send(200, "text/plain", "Done!");
            });

  server.on("/left", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              moveLeft();
              request->send(200, "text/plain", "Done!");
            });

  server.on("/right", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              moveRight();
              request->send(200, "text/plain", "Done!");
            });

  server.onNotFound(notFound);

  server.begin();
}

void loop()
{
}