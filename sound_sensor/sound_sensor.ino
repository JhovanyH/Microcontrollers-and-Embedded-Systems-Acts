const int SOUND = A0;
const int LED = 8;
int threshold = 500;

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int soundValue = analogRead(SOUND);
  Serial.println(soundValue);

  if (soundValue > threshold) {
    digitalWrite(LED, HIGH);
  } else {
    digitalWrite(LED, LOW);
  }
  delay(300);
}
