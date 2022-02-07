#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <WiFi.h>
#include <esp32PWMUtilities.h>
#include <DabbleESP32.h>

Motor Motor1;
Motor Motor2;

void initialize_motors()
{
  Motor1.attach(14, 16, 17);
  Motor2.attach(15, 18, 19);
  Dabble.begin("UGV");
}

Adafruit_MPU6050 mpu;
Adafruit_Sensor *mpu_gyro;

static float offset_z = 0;

static const float degrees_90 = 1.21;

void calibrate_gyro()
{
  int times = 10;
  float offsets[times];
  int t = times;
  while (t--)
  {
    sensors_event_t g;
    mpu_gyro->getEvent(&g);
    offsets[t - 1] = g.gyro.z;
    delay(10);
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
  initialize_motors();
}

float current_angle = 0;

const int loop_ms = 10;

void update_current_angle(int time = loop_ms)
{
  sensors_event_t g;
  mpu_gyro->getEvent(&g);
  current_angle += (g.gyro.z - offset_z) * time / 1000;
  current_angle = fmod(current_angle, degrees_90 * 4);
}

// dir = 1 for forward, -1 for backward
void move(int dir)
{
  Motor1.moveMotor(dir * 2.55 * 100);
  Motor2.moveMotor(dir * 2.55 * 100);
  delay(10);
  Serial.println("Moving " + String(dir));
}

void stop()
{
  Motor1.lockMotor();
  Motor2.lockMotor();
}

void turn_right()
{
  Motor1.moveMotor(2.55 * 100);
  Motor2.moveMotor(-2.55 * 100);
  Serial.println("Turning right");
}

void turn_left()
{
  Motor1.moveMotor(-2.55 * 100);
  Motor2.moveMotor(2.55 * 100);
  Serial.println("Turning left");
}

bool compare(double value1, double value2, int precision)
{
  bool eq = std::abs(value1 - value2) < std::pow(10, -precision);
  return !eq;
}

// dir = 1 for right, -1 for left
void turn_90(int dir)
{
  auto _original_angle = current_angle;
  while (compare(double(current_angle), double(_original_angle + dir * degrees_90), 2))
  {
    Serial.println("Current angle: " + String(current_angle));
    Serial.println("Required angle: " + String(_original_angle + dir * degrees_90));
    if (current_angle > _original_angle + dir * degrees_90)
      turn_left();
    else if (current_angle < _original_angle + dir * degrees_90)
      turn_right();
    else
      break;
    update_current_angle();
    delay(10);
    stop();
  }
}

void process_dabble_input()
{
  Dabble.processInput();
  if (GamePad.isUpPressed())
    move(1);
  else if (GamePad.isDownPressed())
    move(-1);
  else if (GamePad.isLeftPressed())
    turn_90(-1);
  else if (GamePad.isRightPressed())
    turn_90(1);
  else if (GamePad.isCrossPressed())
  {
    Serial.println("Rectangle");
  }
  stop();
}

void loop()
{
  update_current_angle();
  process_dabble_input();
  // Serial.println("Current angle: " + String(current_angle));
  delay(loop_ms);
}
