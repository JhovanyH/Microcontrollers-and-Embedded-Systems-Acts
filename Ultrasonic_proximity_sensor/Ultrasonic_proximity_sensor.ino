const int trig = 9;
const int echo = 10;
const int led = 6;
//added a variable for cleaner coding
const int threshold = 10;

void setup() {
pinMode(trig, OUTPUT);
pinMode(echo, INPUT);
pinMode(led, OUTPUT);
Serial.begin(9600);  //turn on communication between the arduino and computer
}

void loop() {
digitalWrite(trig, LOW);
delayMicroseconds(2);
digitalWrite(trig, HIGH);
delayMicroseconds(10);
digitalWrite(trig, LOW);

long duration = pulseIn(echo, HIGH);  //can hold up to 2.1 billion whole numbers
float distance = duration * 0.034/2;  //sound travels for about 340 centimeters per second, divide to 2 since the signal bounced back
//print out in serial monitor
Serial.print("Distance: ");
Serial.print(distance);
Serial.println(" cm");

if (distance < threshold) {
  int brightness = map(distance, 0, threshold, 255, 0); //use map(value to change, old min, old max, min out, max out)
  brightness = constrain(brightness, 0, 255); //brightness only vary from 0 to 255 based on the distance
  analogWrite(led, brightness); 
} 
else {
  analogWrite(led, 0);
}

}



