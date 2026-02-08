int speed = 100;
int buttonPin = 2;

void setup() {
  // put your setup code here, to run once:
pinMode(buttonPin, INPUT);
pinMode(8, OUTPUT);
pinMode(9, OUTPUT);     
}

void loop() {
  // put your main code here, to run repeatedly:
int buttonStatus = digitalRead(buttonPin);

if (buttonStatus == HIGH) {
  digitalWrite(8, HIGH);
  digitalWrite(9, LOW);
  delay(speed);

  digitalWrite(8, LOW);
  digitalWrite(9, HIGH);
  delay(speed);
}

else {
  digitalWrite(8, LOW);
  digitalWrite(9, LOW);
}
}
