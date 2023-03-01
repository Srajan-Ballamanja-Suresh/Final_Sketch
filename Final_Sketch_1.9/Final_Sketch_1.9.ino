const int irPin1 = 8;
const int irPin2 = 9;
const int relay1 = 4;
const int relay2 = 6;
bool hasEntered = false;
bool hasRun1 = false;
bool hasRun2 = false;

//Variables for timing Bridge
unsigned long previousBridge = 0;
unsigned long currentBridge = 0;

//set time intervals
const unsigned long bridgeInterval = 3500;
const unsigned long irInterval = 100;

void setup() {
  pinMode(irPin1, INPUT);
  pinMode(irPin2, INPUT);
  pinMode(relay1, OUTPUT);
  pinMode(relay2, OUTPUT);
  digitalWrite(relay1, HIGH);
  digitalWrite(relay2, HIGH);
  Serial.begin(9600);
}

void loop() {
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
