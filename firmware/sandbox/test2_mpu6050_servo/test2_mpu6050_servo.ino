#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Servo.h>

Adafruit_MPU6050 mpu;
Servo servo;

void setup() {
  Serial.begin(115200);
  servo.attach(3);
  Wire.begin();

  if (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip. Check wiring!");
    while (1) { delay(10); }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  Serial.println("MPU6050 + Servo control initialized.\n");
  delay(100);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Print all readings
  Serial.print("Accel X: "); Serial.print(a.acceleration.x); Serial.print(" m/s², ");
  Serial.print("Y: "); Serial.print(a.acceleration.y); Serial.print(" m/s², ");
  Serial.print("Z: "); Serial.println(a.acceleration.z); Serial.println(" m/s²");

  Serial.print("Gyro X: "); Serial.print(g.gyro.x); Serial.print(" °/s, ");
  Serial.print("Y: "); Serial.print(g.gyro.y); Serial.print(" °/s, ");
  Serial.print("Z: "); Serial.println(g.gyro.z); Serial.println(" °/s");

  Serial.print("Temperature: "); Serial.print(temp.temperature); Serial.println(" °C");
  Serial.println("--------------------------------");

  // Servo control from Y-axis tilt
  float accelY = a.acceleration.y;
  int servoAngle = map(accelY, -10, 10, 180, 0);
  servoAngle = constrain(servoAngle, 0, 180);
  servo.write(servoAngle);

  delay(200);
}
