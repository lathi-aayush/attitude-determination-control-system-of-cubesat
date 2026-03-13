
[[issue of overloading fast PID calculations]]
# **Step-by-Step Calibration for Your CubeSat Attitude Control**

---

## **Step 0: Setup Checklist**

MPU6050 → Arduino
VCC → 5V
GND → GND
SDA → A4
SCL → A5

L293D MODULE:
A1 → D9
A2 → D10
ENA → D5 (PWM)
VCC → 5V
GND → Common GND (Arduino + Battery -)
+ (Motor Power) → Battery +
MA1 → Motor +
MA2 → Motor -

Before starting, make sure:  
✅ All GNDs are connected together (Arduino, L293D, MPU6050, battery −)  
✅ Arduino powered via USB (or 5V), motor powered via battery  
✅ MPU6050 wired to Arduino (SDA → A4, SCL → A5, VCC → 5V, GND → GND)  
✅ Motor connected to L293D (MA1/MA2), ENA → D5, A1 → D9, A2 → D10  
✅ CubeSat can rotate freely (or test platform)

---

## **Step 1: Upload Test Sketch**

1. Open **Arduino IDE**.
    
2. Copy the **MPU6050_light + PID code** I just gave you.
    
3. Upload it to Arduino.
    
4. Open **Serial Plotter** (Tools → Serial Plotter).
    

✅ You should start seeing **Pitch, Output, Error** numbers scroll.

- If MPU6050 says connection failed → check wiring.
    

---

## **Step 2: Let the MPU6050 Settle**

- Place the CubeSat **flat**.
    
- Wait **10–15 seconds**. The MPU6050_light library internally stabilizes gyro readings.
    
- Make sure your `Pitch` reading is close to **0°**.
    

> If it shows ~0° when flat → perfect.  
> If not, gently rotate and watch Serial Plotter to see full range.

---

## **Step 3: Test Motor Response**

Before PID tuning, check **motor moves correctly**.

1. Open **Arduino Serial Monitor**.
    
2. Upload this tiny sketch to spin motor manually:
    

```cpp
#define ENA 5
#define A1 9
#define A2 10

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
}

void loop() {
  // Forward
  digitalWrite(A1, HIGH);
  digitalWrite(A2, LOW);
  analogWrite(ENA, 100); // slow speed
  delay(2000);

  // Backward
  digitalWrite(A1, LOW);
  digitalWrite(A2, HIGH);
  analogWrite(ENA, 100);
  delay(2000);
}
```

- Motor should spin **forward 2 sec → backward 2 sec**.
    
- ✅ If yes → wiring is correct.
    
- ⚠️ If not → check A1/A2, ENA, motor terminals, battery.
    

---

## **Step 4: Start with PID Tuning**

**Important:** tune **one constant at a time**.

``` c++
#include <Wire.h>
#include <MPU6050_light.h>

MPU6050 mpu(Wire);

// Motor driver pins
#define ENA 5    // PWM speed
#define A1 9     // Motor direction 1
#define A2 10    // Motor direction 2

// PID parameters (start tuning)
float Kp = 4.0;
float Ki = 0.02;
float Kd = 1.5;

float setPoint = 0;      // Target pitch angle (level)
float error, prevError = 0;
float integral = 0;
float derivative;
float output;

unsigned long prevTime = 0;

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
  // Update MPU6050 angles
  mpu.update();
  float pitch = mpu.getAngleX();  // Pitch angle (rotation around X axis)

  // PID computation
  unsigned long currTime = millis();
  float elapsedTime = (currTime - prevTime) / 1000.0;

  error = setPoint - pitch;
  integral += error * elapsedTime;
  derivative = (error - prevError) / elapsedTime;
  output = Kp * error + Ki * integral + Kd * derivative;

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

  // Serial logging for plotter
  Serial.print("Pitch: ");
  Serial.print(pitch);
  Serial.print("\tOutput: ");
  Serial.print(output);
  Serial.print("\tError: ");
  Serial.println(error);
}

```

### Step 4a: Only Kp (Proportional)

1. Set `Ki = 0`, `Kd = 0`.
    
2. Start with `Kp = 1.0`. Upload.
    
3. Gently tilt CubeSat, release → motor should try to correct.
    
4. Slowly **increase Kp** (1 → 2 → 3 → 4 …) until CubeSat oscillates **slightly** but comes back.
    
    - ✅ Oscillates → next step.
        
    - ⚠️ CubeSat spins wildly → reduce Kp.
        

---

### Step 4b: Add Kd (Derivative)

1. Kd helps **smooth oscillations**.
    
2. Start with `Kd = 0.5`. Upload.
    
3. Observe Serial Plotter → oscillations should **dampen faster**.
    
4. Increase gradually until CubeSat **stabilizes without jerky motion**.
    

---

### Step 4c: Add Ki (Integral)

1. Ki removes **small drift** (if CubeSat slowly tilts over time).
    
2. Start small: `Ki = 0.01`. Upload.
    
3. Watch Serial Plotter → pitch should stay at **0°** long-term.
    
4. Increase slightly if needed, **don’t make it too high** → will cause slow oscillations.
    

---

## **Step 5: Observe Serial Plotter**

- **Pitch** → CubeSat angle
    
- **Output** → motor PWM command
    
- **Error** → difference from target
    
- Watch how it stabilizes over time.
    

✅ When it oscillates slightly then settles → PID tuned successfully.

---

## **Step 6: Fine-Tuning Tips**

- CubeSat too slow → increase Kp slightly
    
- Oscillates → increase Kd slightly
    
- Slow drift → increase Ki slightly
    

**Rule of thumb:** small changes, upload, observe, repeat.

---

## **Step 7: Safety**

- Keep CubeSat free to rotate → nothing gets hit.
    
- Motor can spin fast → avoid fingers near it.
    
- If it spins uncontrollably → unplug motor battery, fix PID values.
    

---

If you want, I can now **give you a starting PID set** that works for a small CubeSat with **1 DC motor reaction wheel**, so your CubeSat should **balance on the first try**.

Do you want me to do that?