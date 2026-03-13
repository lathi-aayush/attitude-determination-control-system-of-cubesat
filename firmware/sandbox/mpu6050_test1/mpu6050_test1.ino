#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>

Adafruit_MPU6050 mpu;

// Complementary filter constant (0–1)
const float alpha = 0.98; 

// angles
float pitch = 0.0;
float roll = 0.0;

// timing
unsigned long prevTime = 0;

void setup() {
  Serial.begin(9600);
  while (!Serial) delay(10);

  Wire.begin();
  if (!mpu.begin(0x68)) {
    Serial.println("MPU6050 not found! Check wiring or ID.");
    while (1) { delay(10); }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_4_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("MPU6050 calibration + complementary filter ready.");
  delay(1000);
  prevTime = millis();
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // time difference
  unsigned long currTime = millis();
  float dt = (currTime - prevTime) / 1000.0;
  prevTime = currTime;

  // Convert gyro to deg/s
  float gyroX = g.gyro.x;
  float gyroY = g.gyro.y;
  float gyroZ = g.gyro.z;

  // Calculate angles from accelerometer
  float accelPitch = atan2(-a.acceleration.x, sqrt(a.acceleration.y*a.acceleration.y + a.acceleration.z*a.acceleration.z)) * 180.0 / PI;
  float accelRoll  = atan2(a.acceleration.y, a.acceleration.z) * 180.0 / PI;

  // Complementary filter: combine gyro integration with accel angle
  pitch = alpha * (pitch + gyroX * dt) + (1 - alpha) * accelPitch;
  roll  = alpha * (roll + gyroY * dt)  + (1 - alpha) * accelRoll;

  // Print results
  Serial.print("Pitch: "); Serial.print(pitch, 2);
  Serial.print(" °\tRoll: "); Serial.println(roll, 2);

  delay(20); // ~50 Hz update
}
