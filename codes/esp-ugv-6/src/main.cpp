#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <WiFi.h>
#include <AsyncUDP.h>

const char *ssid = "beingHuman";
const char *password = "1234qwer!@#$QWER";

const auto server_ip = IPAddress(192, 168, 0, 101);
const auto server_port = 8989;

void initialize_WiFi()
{
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

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

AsyncUDP udp;

void connect_udp()
{
  if (!udp.connected())
  {
    if (!udp.connect(server_ip, server_port))
      Serial.println("UDP connect failed");
  }
}

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
  connect_udp();
  if (udp.connected())
  {
    udp.print(data);
  }
}

void setup(void)
{
  Serial.begin(115200);
  while (!Serial)
    delay(10);
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
  initialize_WiFi();
  connect_udp();
  Serial.println("Setup done");
}

const int loop_ms = 10;

void loop()
{
  send_data_to_server();
  delay(loop_ms);
}
