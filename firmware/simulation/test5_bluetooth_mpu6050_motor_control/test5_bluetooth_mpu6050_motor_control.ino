#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

#define ENA 5   // PWM speed
#define A1 9    // Motor direction 1
#define A2 10   // Motor direction 2

// Bluetooth pins (SoftwareSerial)
#include <SoftwareSerial.h>
SoftwareSerial BT(2, 3);  // RX, TX (connect HC-05 TX → pin 2, HC-05 RX → pin 3 via voltage divider)

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
  BT.begin(9600);
  Wire.begin();

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
  float pitch = mpu.getAngleX();

  // --- Bluetooth input ---
  if (BT.available()) {
    char cmd = BT.read();
    if (cmd == 'l') {
      setPoint -= 5;  // rotate slightly left
      Serial.println("↩️ Left command received");
    }
    else if (cmd == 'r') {
      setPoint += 5;  // rotate slightly right
      Serial.println("↪️ Right command received");
    }
  }

  // --- PID ---
  unsigned long currTime = millis();
  float elapsedTime = (currTime - prevTime) / 1000.0;
  if (elapsedTime <= 0) elapsedTime = 0.001;

  error = setPoint - pitch;
  integral += error * elapsedTime;
  integral = constrain(integral, -100, 100);
  derivative = (error - prevError) / elapsedTime;
  output = Kp * error + Ki * integral + Kd * derivative;
  output = constrain(output, -255, 255);

  prevError = error;
  prevTime = currTime;

  // --- Motor control ---
  int speed = constrain(abs(output), 0, 255);
  if (output > 0) {
    digitalWrite(A1, HIGH);
    digitalWrite(A2, LOW);
  } else {
    digitalWrite(A1, LOW);
    digitalWrite(A2, HIGH);
  }
  analogWrite(ENA, speed);

  // --- Serial debug ---
  if (millis() - lastPrint > 100) {
    lastPrint = millis();
    Serial.print("Pitch: "); Serial.print(pitch);
    Serial.print("\tSetPoint: "); Serial.print(setPoint);
    Serial.print("\tOutput: "); Serial.println(output);
  }
}
