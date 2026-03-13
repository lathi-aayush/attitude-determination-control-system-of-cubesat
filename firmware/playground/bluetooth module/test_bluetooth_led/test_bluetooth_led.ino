#include <SoftwareSerial.h>
SoftwareSerial BT(10, 11);  // RX, TX

int greenLED = 2;
int redLED = 3;

void setup() {
  pinMode(greenLED, OUTPUT);
  pinMode(redLED, OUTPUT);
  digitalWrite(greenLED, LOW);
  digitalWrite(redLED, LOW);
  
  Serial.begin(9600);
  BT.begin(9600);
  Serial.println("Bluetooth Ready. Waiting for input...");
}

void loop() {
  if (BT.available()) {
    char data = BT.read();
    Serial.print("Received: ");
    Serial.println(data);

    if (data == 'r') {
      digitalWrite(greenLED, HIGH);
      digitalWrite(redLED, LOW);
    }
    else if (data == 'l') {
      digitalWrite(greenLED, LOW);
      digitalWrite(redLED, HIGH);
    }
  }
}
