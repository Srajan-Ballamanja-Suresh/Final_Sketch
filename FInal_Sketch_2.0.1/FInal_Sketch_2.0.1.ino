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

//Variable for timing servo
unsigned long previousServo = 0;


//Variables for timing Bridge
unsigned long previousBridge = 0;
unsigned long currentTime = 0;

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
  currentTime = millis();
  if (currentTime - previousIr >= irInterval) {
    previousIr = currentTime;
    reading = digitalRead(irPinRoad);
  }
  if (reading == 1) {
    counter++;
  } else {
    fcount = counter;
    counter = 0;
  }
  Serial.println(fcount);
  if (fcount != 0 && fcount <= 300 && !moved) {
    Serial.println("Servo Moved");
    servo.write(90);
    moved = true;
    currentTime = millis();
    previousServo = currentTime;
  }
  if (moved) {
    currentTime = millis();
    if (currentTime - previousServo >= servoDuration) {
      previousServo = currentTime;
      Serial.println("Servo Returned");
      servo.write(0);
      moved = false;
    }
  }
}

void moveBridge() {
  currentTime = millis();
  if (currentTime - previousBridge >= irInterval) {
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
    currentTime = millis();

    if (currentTime - previousBridge >= bridgeInterval) {
      previousBridge = currentTime;
      digitalWrite(relay1, HIGH);
      hasEntered = true;
      hasRun1 = false;
    }
  }
  else if (hasRun2) {
    currentTime = millis();

    if (currentTime - previousBridge >= bridgeInterval) {
      previousBridge = currentTime;
      digitalWrite(relay2, HIGH);
      hasEntered = false;
      hasRun2 = false;
    }
  }
}