#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// Uncomment to enable debug mode
// #define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT(x) Serial.print(x)
#define DEBUG_PRINTLN(x) Serial.println(x)
#else
#define DEBUG_PRINT(x)
#define DEBUG_PRINTLN(x)
#endif

Servo myservo; // Create a Servo object

// Define pins for HC-SR04 ultrasonic sensors
const int TRIG_PIN1 = 5;
const int ECHO_PIN1 = 6;
const int TRIG_PIN2 = 8;
const int ECHO_PIN2 = 9;

// Define pin for buzzer
const int BUZZER_PIN = 4;

const float OBJECT_DISTANCE_THRESHOLD = 5.0; // Threshold distance for object detection
const float WATER_LEVEL_THRESHOLD = 4.0;     // Threshold water level for low water warning
const float HYSTERESIS_MARGIN = 1.0;         // Hysteresis margin for stability
const int containerHeight = 16;              // Height of the container in cm

float measureDistance(int trigPin, int echoPin);

// Initialize I2C LCD (address 0x27, 16 columns x 2 rows)
LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
#ifdef DEBUG
  Serial.begin(9600); // Begin serial communication at 9600 baud for debugging
#endif

  // Set up ultrasonic sensor pins and buzzer pin
  pinMode(TRIG_PIN1, OUTPUT);
  pinMode(ECHO_PIN1, INPUT);
  pinMode(TRIG_PIN2, OUTPUT);
  pinMode(ECHO_PIN2, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  myservo.attach(3); // Attach the servo to pin 3
  myservo.write(90); // Set servo to initial position (180 degrees)

  digitalWrite(BUZZER_PIN, HIGH); // Turn off the buzzer

  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System is Ready"); // Display message
  delay(1000);
  lcd.clear();

  DEBUG_PRINTLN("System is Ready"); // Debug message

  delay(1000);
}

void loop()
{
  // Declare variables
  static bool TapState = false; // State of the tap (open or close)
  float distance1 = 0;
  float distance2 = 0;
  float waterHeight = 0;

  // Measure water level
  distance1 = measureDistance(TRIG_PIN1, ECHO_PIN1);

  // Check for valid reading
  if (distance1 <= 0 || distance1 > containerHeight)
  {
    DEBUG_PRINTLN("Invalid sensor reading, skipping calculation.");
    digitalWrite(BUZZER_PIN, HIGH); // If invalid, then buzzer off
  }
  else
  {
    // Calculate water height
    waterHeight = containerHeight - distance1;

    // Check if water level is low
    if (waterHeight <= WATER_LEVEL_THRESHOLD)
    {
      digitalWrite(BUZZER_PIN, LOW); // turn on buzzer
      DEBUG_PRINTLN("WATER LEVEL IS LOW, BUZZER ON");
    }
    else
    {
      digitalWrite(BUZZER_PIN, HIGH); // turn off buzzer
      DEBUG_PRINTLN("WATER LEVEL IS NORMAL, BUZZER OFF");
    }
  }

  // Detect object near the tap
  distance2 = measureDistance(TRIG_PIN2, ECHO_PIN2);
  if (distance2 <= OBJECT_DISTANCE_THRESHOLD && !TapState) // if object is detected near the tap (about 5 cm) and tap is closed
  {
    TapState = true;
    myservo.write(180); // Open the tap by rotating servo to 90 degrees
    DEBUG_PRINTLN("OBJECT DETECTED, TAP OPEN, AND SERVO SET TO 90 DEGREE");
  }
  else if (distance2 >= (OBJECT_DISTANCE_THRESHOLD + HYSTERESIS_MARGIN) && TapState) // if object is not detected near the tap (about 5 cm + 1 cm) and tap is open
  {
    TapState = false;
    myservo.write(90); // Close the tap by rotating servo to 180 degrees
    DEBUG_PRINTLN("NO OBJECT DETECTED, TAP CLOSE, AND SERVO SET TO 180 DEGREE");
  }

  // Display water level and tap state on LCD
  lcd.setCursor(0, 0);
  lcd.print("                "); // Clear the first line
  lcd.setCursor(0, 0);
  lcd.print("Water:");
  lcd.print(waterHeight);
  lcd.print(" cm");

  lcd.setCursor(0, 1);
  lcd.print("                "); // Clear the second line
  lcd.setCursor(0, 1);
  lcd.print("Tap: ");
  lcd.print(TapState ? "Open" : "Close");

  // for debugging
  DEBUG_PRINT("Water Height: ");
  DEBUG_PRINTLN(waterHeight);
  DEBUG_PRINT("Object Distance: ");
  DEBUG_PRINTLN(distance2);

  // Delay before the next measurement
  delay(1000);
}

float measureDistance(int trigPin, int echoPin)
{
  // Function to measure distance using an ultrasonic sensor
  // Inputs:
  //   trigPin - Trig pin of the HC-SR04 sensor
  //   echoPin - Echo pin of the HC-SR04 sensor
  // Output:
  //   Returns the distance measured in centimeters

  // Send ultrasonic pulse
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Receive echo response
  long duration = pulseIn(echoPin, HIGH);

  // Calculate distance in cm
  float distance = duration * 0.034 / 2;

  // For debugging
  DEBUG_PRINT("Trig Pin: ");
  DEBUG_PRINT(trigPin);
  DEBUG_PRINT(" Echo Pin: ");
  DEBUG_PRINT(echoPin);
  DEBUG_PRINT(" Duration: ");
  DEBUG_PRINT(duration);
  DEBUG_PRINT(" Distance: ");
  DEBUG_PRINTLN(distance);

  return distance; // Return the measured distance
}