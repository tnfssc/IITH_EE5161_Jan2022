# Rectangle Tracer using MPU Gyro sensor

## Trace a perfect rectangle using MPU Gyrosensor

- I tried to use an MPU6050 to track the rotation of the UGV and correct it's path.
- My idea was to turn left if it over turned to right or viceversa till the angle is perfectly correct.
- My intuition was right, but executing it using these motors proved to be harder than I imagined.
- Since the motors lack variable speed, turning very slightly is near impossible.
- That means, the UGV was turning left, overshooting, turning right, overshooting and repeating.
- I am not sure if I can fix this using variable speed motors or maybe at least stepper motors, however it is worth giving a shot.
- [Code available here - esp-ugv-5](https://github.com/tnfssc/IITH_EE5161_Jan2022/tree/main/codes/esp-ugv-5)
