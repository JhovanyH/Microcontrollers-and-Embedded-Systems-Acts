//traffic light simulation code 
//count for 10 seconds and each set of time corresponds to different light colors

//initialize the time which starts at 0
int timer = 0;
void setup() {
//initialize serial communication at 9600 bits per second
Serial.begin(9600);
}

void loop() {
Serial.print("Time:");
Serial.print(timer);
Serial.print("-->");

//phase1: 0 to 3 seconds "GO"
if (timer <= 3) {
  Serial.println("GREEN");
}

//phase 2: 4 to 5 seconds "PREPARE"
else if (timer <= 5) {
  Serial.println("YELLOW");
}

//phase 3: 6 to 9 seconds "STOP"
else if (timer <= 9) {
  Serial.println("RED");
}
//increment by 1 second
delay(1000);
timer++;
//additional condition for reaching 10 seconds
if (timer > 9){
  timer = 0;
  Serial.println("-->Cycle Reset<--");
}
}
