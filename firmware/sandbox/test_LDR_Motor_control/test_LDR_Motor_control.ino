// Automatic LDR -> L293 motor control
// Left LDR  -> A0
// Right LDR -> A1
// Motor (L293): ENA = PWM speed, IN1/IN2 = direction pins
const int LDR_LEFT = A0;
const int LDR_RIGHT = A1;

const int ENA = 5;   // PWM (speed)
const int IN1 = 9;   // direction pin 1
const int IN2 = 10;  // direction pin 2

// Behavior tuning
const int ON_SPEED = 200;     // motor PWM when active (0..255)
const int DEADZONE = 30;      // minimum difference between LDRs to trigger turn
const unsigned long SAMPLE_MS = 150; // loop delay

// Track last applied state to avoid rapid flips
enum MotorState { STOPPED, TURN_LEFT, TURN_RIGHT };
MotorState lastState = STOPPED;

void setup() {
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Start stopped (braked)
  applyMotor(0, STOPPED);
  Serial.println(F("Auto LDR->Motor ready"));
}

void loop() {
  int vL = analogRead(LDR_LEFT);
  int vR = analogRead(LDR_RIGHT);
  int diff = vL - vR; // positive -> left brighter

  // Decide state using deadzone/hysteresis
  MotorState desired;
  if (diff > DEADZONE) desired = TURN_LEFT;
  else if (diff < -DEADZONE) desired = TURN_RIGHT;
  else desired = STOPPED;

  // Only change outputs if desired != lastState (reduces jitter)
  if (desired != lastState) {
    switch (desired) {
      case TURN_LEFT:
        applyMotor(ON_SPEED, TURN_LEFT);
        Serial.print("Action: TURN LEFT | ");
        break;
      case TURN_RIGHT:
        applyMotor(ON_SPEED, TURN_RIGHT);
        Serial.print("Action: TURN RIGHT | ");
        break;
      case STOPPED:
      default:
        applyMotor(0, STOPPED);
        Serial.print("Action: STOPPED | ");
        break;
    }
    lastState = desired;
  }

  // Status print
  Serial.print("L: ");
  Serial.print(vL);
  Serial.print(" R: ");
  Serial.print(vR);
  Serial.print(" Diff: ");
  Serial.println(diff);

  delay(SAMPLE_MS);
}

// apply motor outputs: speed 0 => brake/stop
void applyMotor(int speed, MotorState state) {
  speed = constrain(speed, 0, 255);
  if (speed == 0 || state == STOPPED) {
    // Brake: both direction pins LOW and PWM 0
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, 0);
    return;
  }

  if (state == TURN_RIGHT) {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
  } else { // TURN_LEFT
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
  }
  analogWrite(ENA, speed);
}
