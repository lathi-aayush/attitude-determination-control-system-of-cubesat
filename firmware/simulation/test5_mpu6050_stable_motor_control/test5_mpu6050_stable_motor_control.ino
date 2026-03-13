#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

// Motor control pins
#define ENA 5
#define A1 9
#define A2 10

// PID Controller parameters (optimized for better response)
float Kp = 2.5;        // Increased for better response
float Ki = 0.08;       // Increased for steady state accuracy
float Kd = 0.8;        // Increased for better damping

// Safety and control limits
#define MAX_ANGLE 60.0      // Increased working range (degrees)
#define MAX_SPEED 220       // Increased maximum motor speed
#define MIN_SPEED 15        // Lower minimum speed for sensitivity
#define EMERGENCY_ANGLE 80.0 // Increased emergency stop angle
#define DEADBAND 0.3        // Smaller deadband for better response

// PID variables
float setPoint = 0;
float error, prevError = 0;
float integral = 0;
float derivative;
float output;

// Safety and control variables
bool emergencyStop = false;
bool motorEnabled = true;
unsigned long prevTime = 0;
unsigned long lastPrint = 0;
unsigned long lastStableTime = 0;

// Speed ramping variables
float targetSpeed = 0;
float currentSpeed = 0;
float speedRampRate = 100.0; // Increased speed change per second for faster response

// Stability detection
float stableAngleThreshold = 2.0;
unsigned long stableDuration = 1000; // ms

void setup() {
  Serial.begin(115200);
  Wire.begin();

  // Initialize MPU6050
  byte status = mpu.begin();
  if (status != 0) {
    Serial.println("❌ MPU6050 connection failed!");
    while (1);
  }
  Serial.println("✅ MPU6050 connected successfully!");
  Serial.println("🔧 Starting stable motor control system...");

  // Setup motor pins
  pinMode(ENA, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  
  // Ensure motor starts in stopped state
  stopMotor();

  prevTime = millis();
  lastStableTime = millis();
}

void loop() {
  mpu.update();
  float roll = mpu.getAngleY();  // Changed from getAngleX() to getAngleY()
  
  // Check for emergency conditions
  checkEmergencyConditions(roll);
  
  if (emergencyStop) {
    emergencyStopMotor();
    return;
  }

  // Calculate PID only if motor is enabled and angle is within safe range
  if (motorEnabled && abs(roll) <= MAX_ANGLE) {
    calculatePID(roll);
    controlMotor();
  } else if (abs(roll) > MAX_ANGLE) {
    // Gradually reduce speed when approaching limits
    reduceSpeedGradually();
  } else {
    stopMotor();
  }

  // Update timing
  unsigned long currTime = millis();
  prevTime = currTime;

  // Print debug information
  if (millis() - lastPrint > 150) {
    printDebugInfo(roll);
    lastPrint = millis();
  }
}

void checkEmergencyConditions(float roll) {
  // Emergency stop if angle is too extreme
  if (abs(roll) > EMERGENCY_ANGLE) {
    emergencyStop = true;
    Serial.println("🚨 EMERGENCY STOP: Roll angle too extreme!");
  }
  
  // Reset emergency stop if angle comes back to safe range
  if (emergencyStop && abs(roll) < MAX_ANGLE) {
    emergencyStop = false;
    Serial.println("✅ Emergency cleared, resuming control");
  }
}

void calculatePID(float roll) {
  unsigned long currTime = millis();
  float elapsedTime = (currTime - prevTime) / 1000.0;
  if (elapsedTime <= 0) elapsedTime = 0.001;

  error = setPoint - roll;

  // Integral windup protection with conditional reset
  if (abs(error) > 40) {
    integral = 0; // Reset integral for very large errors
  } else {
    integral += error * elapsedTime;
    // Constrain integral to prevent windup (increased limits)
    integral = constrain(integral, -50, 50);
  }

  derivative = (error - prevError) / elapsedTime;
  
  // Apply PID formula
  output = Kp * error + Ki * integral + Kd * derivative;
  
  // Apply output limits
  output = constrain(output, -MAX_SPEED, MAX_SPEED);

  // Deadband for small errors
  if (abs(error) < DEADBAND) {
    output = 0;
  }

  prevError = error;
}

void controlMotor() {
  targetSpeed = abs(output);
  
  // Ensure minimum speed for small corrections
  if (targetSpeed > 0 && targetSpeed < MIN_SPEED) {
    targetSpeed = MIN_SPEED;
  }
  
  // Apply speed ramping for smooth operation
  rampSpeed();
  
  // Set motor direction and speed
  if (output > 0) {
    digitalWrite(A1, HIGH);
    digitalWrite(A2, LOW);
  } else if (output < 0) {
    digitalWrite(A1, LOW);
    digitalWrite(A2, HIGH);
  } else {
    digitalWrite(A1, LOW);
    digitalWrite(A2, LOW);
  }
  
  analogWrite(ENA, (int)currentSpeed);
}

void rampSpeed() {
  unsigned long currTime = millis();
  float elapsedTime = (currTime - prevTime) / 1000.0;
  
  float speedDifference = targetSpeed - currentSpeed;
  float maxSpeedChange = speedRampRate * elapsedTime;
  
  if (abs(speedDifference) <= maxSpeedChange) {
    currentSpeed = targetSpeed;
  } else {
    currentSpeed += (speedDifference > 0) ? maxSpeedChange : -maxSpeedChange;
  }
  
  // Ensure speed stays within bounds
  currentSpeed = constrain(currentSpeed, 0, MAX_SPEED);
}

void reduceSpeedGradually() {
  // Gradually reduce speed when approaching angle limits
  targetSpeed = MAX_SPEED * (MAX_ANGLE - abs(mpu.getAngleY())) / MAX_ANGLE;
  targetSpeed = constrain(targetSpeed, 0, MAX_SPEED);
  
  rampSpeed();
  
  // Set motor direction based on error sign
  float roll = mpu.getAngleY();
  if (roll > 0) {
    digitalWrite(A1, HIGH);
    digitalWrite(A2, LOW);
  } else {
    digitalWrite(A1, LOW);
    digitalWrite(A2, HIGH);
  }
  
  analogWrite(ENA, (int)currentSpeed);
}

void stopMotor() {
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  analogWrite(ENA, 0);
  currentSpeed = 0;
  targetSpeed = 0;
  integral = 0; // Reset integral when stopping
}

void emergencyStopMotor() {
  digitalWrite(A1, LOW);
  digitalWrite(A2, LOW);
  analogWrite(ENA, 0);
  currentSpeed = 0;
  targetSpeed = 0;
  integral = 0;
  motorEnabled = false;
  
  // Re-enable motor after emergency is cleared
  if (abs(mpu.getAngleY()) < MAX_ANGLE) {
    motorEnabled = true;
  }
}

void printDebugInfo(float roll) {
  Serial.print("Roll: ");
  Serial.print(roll, 1);
  Serial.print("°\tError: ");
  Serial.print(error, 1);
  Serial.print("°\tOutput: ");
  Serial.print(output, 0);
  Serial.print("\tSpeed: ");
  Serial.print(currentSpeed, 0);
  Serial.print("/");
  Serial.print(targetSpeed, 0);
  Serial.print("\tIntegral: ");
  Serial.print(integral, 2);
  
  if (emergencyStop) {
    Serial.print("\t🚨 EMERGENCY");
  } else if (!motorEnabled) {
    Serial.print("\t⏸️ DISABLED");
  } else if (abs(roll) > MAX_ANGLE) {
    Serial.print("\t⚠️ LIMIT");
  } else {
    Serial.print("\t✅ ACTIVE");
  }
  
  Serial.println();
}

// Function to reset system (can be called via serial command)
void resetSystem() {
  emergencyStop = false;
  motorEnabled = true;
  integral = 0;
  currentSpeed = 0;
  targetSpeed = 0;
  prevError = 0;
  Serial.println("🔄 System reset completed");
}
