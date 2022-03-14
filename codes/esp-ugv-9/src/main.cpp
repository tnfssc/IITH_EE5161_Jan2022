#include <WiFi.h>
#include <esp32PWMUtilities.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

#include "html.h"
#include "babel.h"
#include "material-ui.h"
#include "react-dom.h"
#include "react.h"

Adafruit_MPU6050 mpu;

static sensors_vec_t gyro_offsets;
static sensors_vec_t accelero_offsets;

void calibrate()
{
  Serial.println("Calibrating...");
  int times = 10;
  sensors_vec_t a_offsets[times];
  sensors_vec_t g_offsets[times];
  int t = times;
  while (t--)
  {
    sensors_event_t a;
    sensors_event_t g;
    sensors_event_t _t;
    mpu.getEvent(&a, &g, &_t);
    g_offsets[t - 1].x = g.gyro.x;
    g_offsets[t - 1].y = g.gyro.y;
    g_offsets[t - 1].z = g.gyro.z;
    a_offsets[t - 1].x = a.acceleration.x;
    a_offsets[t - 1].y = a.acceleration.y;
    a_offsets[t - 1].z = a.acceleration.z;
    delay(100);
    Serial.print(".");
  }
  Serial.println("");
  times--;
  for (int i = 0; i < times; i++)
  {
    gyro_offsets.x += g_offsets[i].x;
    gyro_offsets.y += g_offsets[i].y;
    gyro_offsets.z += g_offsets[i].z;
    accelero_offsets.x += a_offsets[i].x;
    accelero_offsets.y += a_offsets[i].y;
    accelero_offsets.z += a_offsets[i].z;
  }
  gyro_offsets.x /= times;
  gyro_offsets.y /= times;
  gyro_offsets.z /= times;
  accelero_offsets.x /= times;
  accelero_offsets.y /= times;
  accelero_offsets.z /= times;
}

Motor Motor1;
Motor Motor2;

const char *Wifi_ssid = "beingHuman";
const char *Wifi_password = "1234qwer!@#$QWER";

const char *Apssid = "UGV";
const char *Appassword = "00000000";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

void send_data_to_server()
{
  sensors_event_t a;
  sensors_event_t g;
  sensors_event_t _t;
  mpu.getEvent(&a, &g, &_t);
  char data[127];
  const int time = micros();
  sprintf(
      data,
      "Time: %d\tAcceleration: %f, %f, %f\tGyro: %f, %f, %f\n",
      time,
      a.acceleration.x - accelero_offsets.x,
      a.acceleration.y - accelero_offsets.y,
      a.acceleration.z - accelero_offsets.z,
      g.gyro.x - gyro_offsets.x,
      g.gyro.y - gyro_offsets.y,
      g.gyro.z - gyro_offsets.z);
  ws.textAll(data);
}

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

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len)
{
  AwsFrameInfo *info = (AwsFrameInfo *)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT)
  {
    data[len] = 0;
    if (strcmp((char *)data, "forward") == 0)
    {
      moveForward();
    }
    else if (strcmp((char *)data, "backward") == 0)
    {
      moveBackward();
    }
    else if (strcmp((char *)data, "right") == 0)
    {
      moveRight();
    }
    else if (strcmp((char *)data, "left") == 0)
    {
      moveLeft();
    }
    else
    {
      stop();
    }
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len)
{
  switch (type)
  {
  case WS_EVT_CONNECT:
    Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
    break;
  case WS_EVT_DISCONNECT:
    Serial.printf("WebSocket client #%u disconnected\n", client->id());
    break;
  case WS_EVT_DATA:
    handleWebSocketMessage(arg, data, len);
    break;
  case WS_EVT_PONG:
  case WS_EVT_ERROR:
    break;
  }
}

void setup()
{
  Serial.begin(9600);
  Motor1.attach(14, 16, 17);
  Motor2.attach(15, 18, 19);

  if (!mpu.begin())
    Serial.println("Failed to find MPU6050 chip");
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G);
  delay(100);
  calibrate();
  char data[127];
  sprintf(data, "Gyro offsets: %f, %f, %f\n", gyro_offsets.x, gyro_offsets.y, gyro_offsets.z);
  Serial.println(data);
  sprintf(data, "Accelero offsets: %f, %f, %f\n", accelero_offsets.x, accelero_offsets.y, accelero_offsets.z);
  Serial.println(data);
  delay(100);

  Serial.println("Configuring access point...");
  WiFi.mode(WIFI_AP);
  WiFi.softAP(Apssid, Appassword);
  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  // Serial.println("");
  // delay(1500);
  // Serial.println("connecting to Wifi:");
  // Serial.println(Wifi_ssid);
  // WiFi.begin(Wifi_ssid, Wifi_password);
  // while (WiFi.status() != WL_CONNECTED)
  // {
  //   delay(500);
  //   Serial.print(".");
  // }
  // Serial.println("");
  // Serial.println("WiFi connected");
  // Serial.println("IP address: ");
  // Serial.println(WiFi.localIP());
  // Serial.println("");

  ws.onEvent(onEvent);
  server.addHandler(&ws);
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", index_html); });
  server.on("/babel-min.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", babel_min); });
  server.on("/material-ui.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", material_ui); });
  server.on("/react-dom.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", react_dom); });
  server.on("/react.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/html", react); });
  server.begin();
}

void loop()
{
  ws.cleanupClients();
  ws.textAll(String("Awesome"));
}