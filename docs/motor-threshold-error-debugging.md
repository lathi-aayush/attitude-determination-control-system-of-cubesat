## ⚙️ 1. The Real Cause: “Integral Windup + Saturation”

Once your CubeSat tilts beyond a few degrees, the PID output becomes large → your `output` quickly hits **±255 limit**.  
At that point:

- Motor is already at full speed.
    
- But since the pitch keeps changing, the **integral** keeps accumulating.
    
- When the CubeSat comes back, the integral term is still huge → output sign flips → motor reverses **violently**.
    
- The system enters oscillation or complete loss of control.
    

This is called **Integral Windup**, and it’s what’s making your motor go berserk after some angle.

---

## 🧠 2. Physical Limitation: Reaction Wheel Can’t Handle Large Angles

A single DC motor acting as a reaction wheel cannot bring back the CubeSat from large angles (e.g., ±20° or more).  
It’s meant for **small attitude corrections (±3°)** only.

When your CubeSat tilts too far:

- Gravity torque overcomes the small angular momentum of your motor.
    
- So the motor keeps spinning faster trying to fight it, but orientation doesn’t improve → output saturates.
    

That’s why you see it **“loses control after some threshold.”**

---

## ✅ 3. Step-by-Step Fixes

### 🔹 Step 1: **Add a Deadband**

Prevent motor from trying to correct very small or very large errors.

```cpp
if (abs(error) < 1.5) output = 0; // small error, don’t correct
if (abs(error) > 15) output = 0; // too tilted, give up control
```

This prevents wild spinning when the CubeSat tips too much.

---

### 🔹 Step 2: **Reset Integral When Error Is Large**

Avoid accumulation during big tilts.

```cpp
if (abs(error) > 10) integral = 0;
```

This ensures that when the CubeSat goes beyond control range, PID resets and avoids runaway.

---

### 🔹 Step 3: **Use Smaller Ki and Slightly Higher Kd**

Try:

```cpp
Kp = 2.0;
Ki = 0.05;
Kd = 0.6;
```

Ki too high causes runaway; Kd provides damping.

---

### 🔹 Step 4: **Optional – Soft Start**

Prevents sudden motor jerk on startup:

```cpp
analogWrite(ENA, constrain(speed, 0, 180)); // limit initial PWM
```

---

## 🧩 4. Your Fixed Code Snippet

Here’s your **stabilized version** — paste and upload directly:

```cpp
#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

#define ENA 5
#define A1 9
#define A2 10

float Kp = 2.0;
float Ki = 0.05;
float Kd = 0.6;

float setPoint = 0;
float error, prevError = 0;
float integral = 0;
float derivative;
float output;

unsigned long prevTime = 0;
unsigned long lastPrint = 0;

void setup() {
  Serial.begin(115200);
  Wire.begin();

  byte status = mpu.begin();
  if (status != 0) {
    Serial.println("❌ MPU6050 connection failed!");
    while (1);
  }
  Serial.println("✅ MPU6050 connected successfully!");

  pinMode(ENA, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);

  prevTime = millis();
}

void loop() {
  mpu.update();
  float pitch = mpu.getAngleX();

  unsigned long currTime = millis();
  float elapsedTime = (currTime - prevTime) / 1000.0;
  if (elapsedTime <= 0) elapsedTime = 0.001;

  error = setPoint - pitch;

  // Reset integral when angle is too large
  if (abs(error) > 10) integral = 0;

  integral += error * elapsedTime;
  if (integral > 50) integral = 50;
  if (integral < -50) integral = -50;

  derivative = (error - prevError) / elapsedTime;
  output = Kp * error + Ki * integral + Kd * derivative;

  if (output > 255) output = 255;
  if (output < -255) output = -255;

  // Deadband (ignore very small errors)
  if (abs(error) < 1.5 || abs(error) > 15) output = 0;

  int speed = constrain(abs(output), 0, 255);

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

  analogWrite(ENA, speed);

  prevError = error;
  prevTime = currTime;

  if (millis() - lastPrint > 50) {
    lastPrint = millis();
    Serial.print("Pitch: "); Serial.print(pitch);
    Serial.print("\tError: "); Serial.print(error);
    Serial.print("\tOutput: "); Serial.println(output);
  }
}
```

---

## 🎯 Final Tuning Tip

- Begin flat → CubeSat level.
    
- Tilt slightly and observe correction.
    
- If it **oscillates**, reduce Kp or increase Kd.
    
- If it’s **too slow**, increase Kp slightly.
    
- If it **drifts or ramps uncontrollably**, reduce Ki further.
    

