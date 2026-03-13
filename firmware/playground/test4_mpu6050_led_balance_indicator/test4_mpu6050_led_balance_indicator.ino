#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

// LED pins
int LED_L1 = 2;
int LED_L2 = 3;
int LED_R1 = 4;
int LED_R2 = 5;

void setup() {
  Serial.begin(9600);
  Wire.begin();

  byte status = mpu.begin();
  if (status != 0) {
    Serial.print("MPU6050 connection failed! Status: ");
    Serial.println(status);
    while (1);
  }

  Serial.println("Calibrating MPU6050...");
  delay(1000);
  mpu.calcOffsets();
  Serial.println("Calibration done!");

  pinMode(LED_L1, OUTPUT);
  pinMode(LED_L2, OUTPUT);
  pinMode(LED_R1, OUTPUT);
  pinMode(LED_R2, OUTPUT);
}

void loop() {
  mpu.update();
  float roll = mpu.getAngleX();   // Roll angle in degrees

  Serial.println(roll); // For Serial Plotter or debugging

  // Turn all LEDs off first
  digitalWrite(LED_L1, LOW);
  digitalWrite(LED_L2, LOW);
  digitalWrite(LED_R1, LOW);
  digitalWrite(LED_R2, LOW);

  // LEFT tilt (negative roll)
  if (roll < -10 && roll >= -30) {
    digitalWrite(LED_L1, HIGH);
  }
  else if (roll < -30) {
    digitalWrite(LED_L1, HIGH);
    digitalWrite(LED_L2, HIGH);
  }

  // RIGHT tilt (positive roll)
  else if (roll > 10 && roll <= 30) {
    digitalWrite(LED_R1, HIGH);
  }
  else if (roll > 30) {
    digitalWrite(LED_R1, HIGH);
    digitalWrite(LED_R2, HIGH);
  }

  delay(10);
}
