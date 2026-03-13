#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);
unsigned long timer = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  byte status = mpu.begin();
  if (status != 0) {
    Serial.print("MPU6050 connection failed! Status: ");
    Serial.println(status);
    while (1);
  }

  Serial.println("MPU6050 connected, calibrating...");
  delay(1000);
  mpu.calcOffsets();  // automatic gyro/accel calibration
  Serial.println("Calibration done!");
}

void loop() {
  mpu.update();

  // print only roll (angle in degrees)
  Serial.println(mpu.getAngleX());   // X-axis = Roll

  delay(10); // ~100Hz update rate
}
