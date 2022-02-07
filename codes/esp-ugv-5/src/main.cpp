#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;
Adafruit_Sensor *mpu_gyro;

static float offset_z = 0;

void calibrate_gyro()
{
  int times = 20;
  float offsets[times];
  int t = times;
  while (t--)
  {
    sensors_event_t g;
    mpu_gyro->getEvent(&g);
    offsets[t - 1] = g.gyro.z;
    delay(100);
  }
  times--;
  for (int i = 0; i < times; i++)
    offset_z += offsets[i];
  offset_z /= times;
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
  mpu_gyro = mpu.getGyroSensor();
  delay(100);
  calibrate_gyro();
  Serial.println("Calibration done. Offset: " + String(offset_z));
  delay(100);
}

float current_angle = 0;

void update_current_angle()
{
  sensors_event_t g;
  mpu_gyro->getEvent(&g);
  current_angle += (g.gyro.z - offset_z) * 0.1;
}

void loop()
{
  sensors_event_t g;
  mpu_gyro->getEvent(&g);
  update_current_angle();
  Serial.println("Current angle: " + String(current_angle));
  delay(100);
}
