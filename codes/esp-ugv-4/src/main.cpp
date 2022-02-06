
#include <Arduino.h>
#include <ArduinoOTA.h>
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

  Serial.println("Connecting to WiFi...");
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
                request->send(200, "text/plain", "Done!"); });

  server.on("/backward", HTTP_GET, [](AsyncWebServerRequest *request)
            {
                moveBackward();
                request->send(200, "text/plain", "Done!"); });

  server.on("/left", HTTP_GET, [](AsyncWebServerRequest *request)
            {
                moveLeft();
                request->send(200, "text/plain", "Done!"); });

  server.on("/right", HTTP_GET, [](AsyncWebServerRequest *request)
            {
                moveRight();
                request->send(200, "text/plain", "Done!"); });

  server.onNotFound(notFound);

  server.begin();

  ArduinoOTA
      .onStart([]()
               {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      server.end();
      SPIFFS.end();
      Serial.println("Start updating " + type); })
      .onEnd([]()
             { Serial.println("\nEnd"); })
      .onProgress([](unsigned int progress, unsigned int total)
                  { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); })
      .onError([](ota_error_t error)
               {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed"); })
      .setPort(3232);

  ArduinoOTA.begin();
}

void loop()
{
  ArduinoOTA.handle();
  delay(100);
}