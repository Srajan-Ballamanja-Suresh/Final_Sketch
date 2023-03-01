#include <Servo.h>
const int irPinRoad = 11;
const int servoPin = 10;
const unsigned long irInterval = 100;
const unsigned long servoDuration = 3000;
bool moved = false;

// Variables for timing
unsigned long previousTime = 0;
unsigned long currentTime = 0;
unsigned long previousServo = 0;
unsigned long currentServo = 0;

// Variables for calculating speed
int counter = 0;
float speed = 0;
int reading = 0;
int fcount = 0;

Servo servo;

void setup() {
  pinMode(irPinRoad, INPUT);
  //DEBUG Serial
  Serial.begin(9600);

  servo.attach(servoPin);
  servo.write(0);
}

void loop() {
  currentTime = millis();
  if (currentTime - previousTime >= irInterval) {
    previousTime = currentTime;
    reading = digitalRead(irPinRoad);
  }
  if (reading == HIGH) {
    counter++;
  } else {
    fcount = counter;
    counter = 0;
  }
  Serial.println(fcount);
  if (fcount != 0 && fcount <= 300) {
    Serial.println("Servo Moved");
    servo.write(90);
    moved = true;
  }
  if (moved) {
    currentServo = millis();
    if (currentServo - previousServo >= servoDuration) {
      previousServo = currentServo;
      Serial.println("Servo Returned");
      servo.write(0);
      moved = false;
    }
  }
}
