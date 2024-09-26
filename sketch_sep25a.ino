#include <SPI.h>
#include <ADXL362.h>
#include <Servo.h>

#define CS 10 // Chip select pin
#define SERVO_PIN 8 // Pin for the servo
#define FALL_THRESHOLD 100 // Adjusted threshold to detect free fall
#define MOVE_DELAY 1000 // Delay to wait before returning to the original position

ADXL362 accelerometer; // Creation of the object
Servo myServo; // Create a Servo object
int currentServoAngle = 0; // Track the current angle of the servo (start at 0 degrees)
bool servoMoved = false; // Flag to check if the servo has moved
bool movingToRight = true; // Direction flag to track servo movement

void setup() {
  Serial.begin(9600); // Initialization of serial communication
  accelerometer.begin(CS); // Initialization of the accelerometer
  accelerometer.beginMeasure(); // Initialization of the measurement
  
  myServo.attach(SERVO_PIN); // Attach the servo to pin 8
  myServo.write(currentServoAngle); // Start the servo at the neutral position (0 degrees)

  // Move the servo to 0 degrees first (desired starting position)
  myServo.write(0); // Move to 0 degrees
  delay(MOVE_DELAY); // Wait for a specified time
  
  // Move the servo to 90 degrees and wait
  myServo.write(90); // Move to 90 degrees
  delay(MOVE_DELAY); // Wait for a specified time
}

void loop() {
  // Acquisition of accelerometer data
  int x, y, z, t;
  accelerometer.readXYZTData(x, y, z, t);

  // Display raw data
  Serial.print("x = ");
  Serial.print(x);
  Serial.print("\t y = ");
  Serial.print(y);
  Serial.print("\t z = ");
  Serial.print(z);
  Serial.print("\t t = ");
  Serial.println(t);

  // Check if the z value indicates a fall and if the servo hasn't moved yet
  if (abs(z) < FALL_THRESHOLD && !servoMoved) {
    Serial.println("Fall detected! Moving servo...");

    if (movingToRight) {
      myServo.write(90); // Move to 90 degrees
    } else {
      myServo.write(0); // Move back to 0 degrees
    }
    
    delay(MOVE_DELAY); // Wait for a specified time before returning

    // Toggle the direction for the next movement
    movingToRight = !movingToRight; // Switch direction
    servoMoved = true; // Set the flag to indicate the servo has moved
  }

  // Check if the servo has reached the target angle and reset the flag
  if (servoMoved && (myServo.read() == (movingToRight ? 90 : 0))) {
    servoMoved = false; // Reset the flag to allow the servo to move again
  }

  // Wait a bit before the next acquisition
  delay(100);
}
