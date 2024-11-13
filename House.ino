#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define TRIG_PIN 13
#define ECHO_PIN 14
#define LED_LOW 27
#define LED_MEDIUM 26
#define LED_HIGH 32
#define BUZZER_PIN 25
#define RELAY_PIN 33 // Pin for relay control

LiquidCrystal_I2C lcd(0x27, 16, 2); // Adjust address as needed

long duration;
int distance;

// Water level thresholds (in cm)
int lowLevelMax = 40;    // Max distance for LOW level
int lowLevelMin = 20;    // Min distance for LOW level
int mediumLevelMax = 20; // Max distance for MEDIUM level
int mediumLevelMin = 10; // Min distance for MEDIUM level
int highLevelMax = 10;   // Max distance for HIGH level
int highLevelMin = 1;    // Min distance for HIGH level

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(LED_LOW, OUTPUT);
  pinMode(LED_MEDIUM, OUTPUT);
  pinMode(LED_HIGH, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT); // Set relay pin as OUTPUT


  digitalWrite(RELAY_PIN, HIGH);

  // Initialize the LCD with columns and rows
  lcd.begin(16, 2); // Specify 16 columns and 2 rows
  lcd.backlight(); 
  Serial.begin(9600);
}

void loop() {
  // Trigger ultrasonic sensor
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  
  // Calculate distance
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration * 0.034 / 2;

  Serial.print("Distance: ");
  Serial.println(distance);
  
  // Display distance on LCD
  lcd.setCursor(0, 0);
  lcd.print("Distance: ");
  lcd.print(distance);
  lcd.print(" cm   "); // Clear any old data

  // Control LEDs, buzzer, and relay based on water level
  if (distance >= lowLevelMin && distance <= lowLevelMax) {
    lcd.setCursor(0, 1);
    lcd.print("Level: LOW  ");
    digitalWrite(LED_LOW, HIGH);
    digitalWrite(LED_MEDIUM, LOW);
    digitalWrite(LED_HIGH, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    
    // Turn ON relay at low level
    digitalWrite(RELAY_PIN, LOW);  // Relay ON
    Serial.println("Relay ON: LOW level detected");
  }
  else if (distance >= mediumLevelMin && distance < lowLevelMin) {
    lcd.setCursor(0, 1);
    lcd.print("Level: MEDIUM");
    digitalWrite(LED_LOW, LOW);
    digitalWrite(LED_MEDIUM, HIGH);
    digitalWrite(LED_HIGH, LOW);
    digitalWrite(BUZZER_PIN, LOW);
    Serial.println("LED MEDIUM ON");
  }
  else if (distance >= highLevelMin && distance < mediumLevelMin) {
    lcd.setCursor(0, 1);
    lcd.print("Level: HIGH  ");
    digitalWrite(LED_LOW, LOW);
    digitalWrite(LED_MEDIUM, LOW);
    digitalWrite(LED_HIGH, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);  // Activate buzzer for HIGH level
    Serial.println("LED HIGH ON");
  }
  else if (distance < highLevelMin) {
    lcd.setCursor(0, 1);
    lcd.print("Level: CRITICAL"); // Display alarm status
    digitalWrite(LED_LOW, LOW);
    digitalWrite(LED_MEDIUM, LOW);
    digitalWrite(LED_HIGH, HIGH);
    digitalWrite(BUZZER_PIN, HIGH);  // Trigger alarm
    
    // Turn OFF relay at high level
    digitalWrite(RELAY_PIN,HIGH);    // Relay OFF
    Serial.println("Relay OFF: HIGH level detected");
    Serial.println("ALARM TRIGGERED");
  }
  
  delay(1000);  // Wait for 1 second before measuring again
}
