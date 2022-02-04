#include <Arduino.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <esp32PWMUtilities.h>
#include <ESPAsyncWebServer.h>

const char *ssid = "beingAlien";
const char *password = "1234qwer!@#$QWER";

AsyncWebServer server(80);

void setup()
{
  Serial.begin(115200);
  if (!SPIFFS.begin())
  {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }
  Serial.println(WiFi.localIP());

  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/webui/build/index.html", "text/html"); });
  server.on("/static/js/main.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/webui/build/static/js/main.js", "text/javascript"); });
  server.begin();
}

void loop()
{
}
