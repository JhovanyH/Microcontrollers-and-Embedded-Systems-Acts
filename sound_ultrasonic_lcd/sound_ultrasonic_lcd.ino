#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// --- Pin Definitions ---
// Ultrasonic Sensor
const int TRIG = 9;
const int ECHO = 10;
const int OCCUPANCY_LED = 8;

// Sound Sensor
const int SOUND = A0;
const int NOISE_LED = 7;
const int BUZZER = 6;

// --- Thresholds ---
const int DISTANCE_THRESHOLD = 50; // Distance in cm to trigger occupancy
const int NOISE_THRESHOLD = 500;   // Analog value to trigger noise warning

// --- LCD Setup ---
// 0x27 is the default I2C address for most 16x2 LCDs. 
// (If your screen is blank, you may need to change this to 0x3F)
LiquidCrystal_I2C lcd(0x27, 16, 2); 

void setup() {
  // Initialize Serial Monitor for data logging
  Serial.begin(9600);
  
  // Set up Ultrasonic pins
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(OCCUPANCY_LED, OUTPUT);
  
  // Set up Sound Sensor and alert pins
  pinMode(NOISE_LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Smart Library");
  lcd.setCursor(0, 1);
  lcd.print("System Starting");
  delay(2000); // Display welcome message for 2 seconds
  lcd.clear();
  
  // Print CSV Header for data analysis
  Serial.println("Time(ms),Distance(cm),SoundLevel,Occupied,NoiseWarning");
}

void loop() {
  // ==========================================
  // 1. READ ULTRASONIC SENSOR (OCCUPANCY)
  // ==========================================
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  long duration = pulseIn(ECHO, HIGH);
  float distance = duration * 0.034 / 2;
  
  // Determine Occupancy state
  bool isOccupied = false;
  if (distance > 0 && distance < DISTANCE_THRESHOLD) {
    isOccupied = true;
    digitalWrite(OCCUPANCY_LED, HIGH);
  } else {
    digitalWrite(OCCUPANCY_LED, LOW);
  }

  // ==========================================
  // 2. READ SOUND SENSOR (NOISE)
  // ==========================================
  int soundValue = analogRead(SOUND);
  
  // Determine Noise state
  bool isNoisy = false;
  if (soundValue > NOISE_THRESHOLD) {
    isNoisy = true;
    digitalWrite(NOISE_LED, HIGH);
    digitalWrite(BUZZER, HIGH);
  } else {
    digitalWrite(NOISE_LED, LOW);
    digitalWrite(BUZZER, LOW);
  }

  // ==========================================
  // 3. UPDATE LCD DISPLAY
  // ==========================================
  // Row 0: Occupancy Status
  lcd.setCursor(0, 0);
  if (isOccupied) {
    lcd.print("Seat: OCCUPIED  "); // Extra spaces overwrite old characters
  } else {
    lcd.print("Seat: EMPTY     ");
  }

  // Row 1: Noise Status
  lcd.setCursor(0, 1);
  if (isNoisy) {
    lcd.print("Noise: TOO LOUD!");
  } else {
    lcd.print("Noise: QUIET    ");
  }

  // ==========================================
  // 4. DATA LOGGING (SERIAL OUTPUT)
  // ==========================================
  // Format: Time, Distance, SoundValue, OccupiedState(1/0), NoiseState(1/0)
  Serial.print(millis());
  Serial.print(",");
  Serial.print(distance);
  Serial.print(",");
  Serial.print(soundValue);
  Serial.print(",");
  Serial.print(isOccupied);
  Serial.print(",");
  Serial.println(isNoisy);

  // Small delay for system stability and readability
  delay(500);
}