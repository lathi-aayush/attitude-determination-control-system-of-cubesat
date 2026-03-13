#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

// Motor driver pins
#define ENA 5  // PWM speed
#define A1 9   // Motor direction 1
#define A2 10  // Motor direction 2

// PID parameters
float Kp = 3.0;
float Ki = 0.5;
float Kd = 0.0;

float setPoint = 0;  // Target pitch angle (level)
float error, prevError = 0;
float integral = 0;
float derivative;
float output;

unsigned long prevTime = 0;
unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Initialize MPU6050
  byte status = mpu.begin();
  if (status == 0) {
    Serial.println("✅ MPU6050 connected successfully!");
  } else {
    Serial.println("❌ MPU6050 connection failed! Check wiring.");
    while (1);
  }

  pinMode(ENA, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);

  prevTime = millis();
}

void loop() {
  mpu.update();
  float pitch = mpu.getAngleX();  // Pitch angle

  // Time calculation
  unsigned long currTime = millis();
  float elapsedTime = (currTime - prevTime) / 1000.0;
  if (elapsedTime <= 0) elapsedTime = 0.001;  // prevent division by zero

  // PID calculation
  error = setPoint - pitch;

  integral += error * elapsedTime;
  // Limit integral to prevent runaway
  if (integral > 100) integral = 100;
  if (integral < -100) integral = -100;

  derivative = (error - prevError) / elapsedTime;

  output = Kp * error + Ki * integral + Kd * derivative;

  // Limit output
  if (output > 255) output = 255;
  if (output < -255) output = -255;

  prevError = error;
  prevTime = currTime;

  // Motor control
  int speed = constrain(abs(output), 0, 255);

  if (output > 0) {
    digitalWrite(A1, HIGH);
    digitalWrite(A2, LOW);
  } else {
    digitalWrite(A1, LOW);
    digitalWrite(A2, HIGH);
  }
  analogWrite(ENA, speed);

  // Serial logging (slower, every 50ms)
  if (millis() - lastPrint > 50) {
    lastPrint = millis();
    Serial.print("Pitch: "); Serial.print(pitch);
    Serial.print("\tOutput: "); Serial.print(output);
    Serial.print("\tError: "); Serial.println(error);
  }
}
