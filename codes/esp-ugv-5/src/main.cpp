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

static long gyro_time = 0;

void calibrate_gyro()
{
  int times = 10;
  float offsets[times];
  long gyro_times[times];
  int t = times;
  while (t--)
  {
    sensors_event_t g;
    long cur = micros();
    mpu_gyro->getEvent(&g);
    cur -= micros();
    offsets[t - 1] = g.gyro.z;
    gyro_times[t - 1] = cur;
    delay(10);
  }
  times--;
  for (int i = 0; i < times; i++)
  {
    offset_z += offsets[i];
    gyro_time += gyro_times[i];
  }
  offset_z /= times;
  gyro_time /= times;
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
  Serial.println(current_angle);
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

void rotate_right()
{
  Motor1.moveMotor(2.55 * 100);
  Motor2.moveMotor(-2.55 * 100);
  Serial.println("Rotating right");
}

void rotate_left()
{
  Motor1.moveMotor(-2.55 * 100);
  Motor2.moveMotor(2.55 * 100);
  Serial.println("Rotating left");
}

void turn_left()
{
  Motor2.moveMotor(2.55 * 100);
  Motor1.lockMotor();
  Serial.println("Turning left");
}

void turn_right()
{
  Motor1.moveMotor(2.55 * 100);
  Motor2.lockMotor();
  Serial.println("Turning right");
}

bool compare(double value1, double value2, int precision)
{
  bool eq = std::abs(value1 - value2) < std::pow(10, -precision);
  return !eq;
}

void go_straight_line()
{
  int l = 500;
  while (l -= 2)
  {
    if (compare(current_angle, 0, 2))
    {
      if (current_angle > 0)
        turn_left();
      else
        turn_right();
      l++;
    }
    else
    {
      move(1);
    }
    delayMicroseconds(loop_ms * 1000 - gyro_time);
    update_current_angle();
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
    rotate_left();
  else if (GamePad.isRightPressed())
    rotate_right();
  else if (GamePad.isCrossPressed())
  {
    Serial.println("Go Straight Line");
    go_straight_line();
  }
  else if (GamePad.isTrianglePressed())
  {
    move(1);
    delay(1000);
    move(-1);
    delay(1000);
    stop();
  }
  else if (GamePad.isCirclePressed())
  {
    Serial.println("Reset angle");
    current_angle = 0;
  }
  stop();
}

void loop()
{
  process_dabble_input();
  delay(loop_ms);
}
