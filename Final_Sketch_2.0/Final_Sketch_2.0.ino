#include <Servo.h>

//pins of the modules
const int irPinRoad = 11;
const int servoPin = 10;
const int irPin1 = 8;
const int irPin2 = 9;
const int relay1 = 4;
const int relay2 = 6;

Servo servo;

//set time intervals
const unsigned long bridgeInterval = 3500;
const unsigned long irInterval = 100;
const unsigned long servoDuration = 3000;

//check variables
bool hasEntered = false;
bool hasRun1 = false;
bool hasRun2 = false;
bool moved = false;

// Variables for timing ir of road
unsigned long previousIr = 0;
unsigned long currentIr = 0;

//Variable for timing servo
unsigned long previousServo = 0;
unsigned long currentServo = 0;

//Variables for timing Bridge
unsigned long previousBridge = 0;
unsigned long currentBridge = 0;

// Variables for calculating speed
int counter = 0;
int reading = 0;
int fcount = 0;

void setup() {
  
  //bridge
  pinMode(irPin1, INPUT);
  pinMode(irPin2, INPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  
  //road
  pinMode(irPinRoad, INPUT);
  
  //DEBUG Serial
  Serial.begin(9600);

  //set servo
  servo.attach(servoPin);
  servo.write(0);
}

void loop() {
  carStopper();
  moveBridge();
}


void carStopper(){
  currentIr = millis();
  if (currentIr - previousIr >= irInterval) {
    previousIr = currentIr;
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

void moveBridge() {
  currentBridge = millis();
  if (currentBridge - previousBridge >= irInterval) {
    if (digitalRead(irPin1) == LOW || digitalRead(irPin2) == LOW) {
      if (hasEntered) {
        hasRun2 = true;
        digitalWrite(relay2, LOW);
      } else {
        hasRun1 = true;
        digitalWrite(relay1, LOW);
      }
    }
  }
  if (hasRun1) {
    currentBridge = millis();

    if (currentBridge - previousBridge >= bridgeInterval) {
      previousBridge = currentBridge;
      digitalWrite(relay1, HIGH);
      hasEntered = true;
      hasRun1 = false;
    }
  }
  else if (hasRun2) {
    currentBridge = millis();

    if (currentBridge - previousBridge >= bridgeInterval) {
      previousBridge = currentBridge;
      digitalWrite(relay2, HIGH);
      hasEntered = false;
      hasRun2 = false;
    }
  }
}