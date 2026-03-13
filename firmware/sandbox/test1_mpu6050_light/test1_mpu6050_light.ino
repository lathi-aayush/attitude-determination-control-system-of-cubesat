#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

void setup() {
  Serial.begin(9600);
  Wire.begin();

  // Initialize MPU6050
  mpu.begin();
  Serial.println("MPU6050_light initialized");

  // Calibrate gyro at startup (keep sensor still for a few seconds)
  mpu.calcGyroOffsets();  // ⚡ no arguments now
  Serial.println("Gyro calibrated. Move the sensor now!");
}

void loop() {
  // Update sensor readings
  mpu.update();

  // Get angles in degrees
  float pitch = mpu.getAngleX();  // Rotation around X-axis
  // float roll = mpu.getAngleY();   // Rotation around Y-axis
  // float yaw = mpu.getAngleZ();    // Rotation around Z-axis (integrated gyro, can drift)

  // Print raw pitch, roll, yaw
  Serial.print("Roll (X-axis): ");
  Serial.print(pitch);
  Serial.print("\n");
  // Serial.print("\tPitch (Y-axis): ");
  // Serial.print(roll);
  // Serial.print("\tYaw (Z-axis): ");
  // Serial.println(yaw);

  // Serial.print(pitch);
  // Serial.print(",");
  // Serial.print(roll);
  // Serial.print(",");
  // Serial.println(yaw);


  delay(100);  // Update 5 times per second
}
