const int irPin1 = 8;
const int irPin2 = 11;
const int relay1 = 4;
const int relay2 = 6;
bool hasEntered = false;

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
  if(digitalRead(irPin1) == HIGH|| digitalRead(irPin2) == HIGH){
    if(hasEntered){
      digitalWrite(relay2, LOW);
      delay(3500);
      digitalWrite(relay2, HIGH);
      hasEntered = false;
    } else {
      hasEntered = true;
      digitalWrite(relay1, LOW);
      delay(3500);
      digitalWrite(relay1, HIGH);
    }
  }
}
