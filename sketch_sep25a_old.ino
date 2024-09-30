#include <SPI.h>
#include <ADXL362.h>
#include <Servo.h>

#define CS 10
#define SERVO_PIN 8 // Servons pin
#define FALL_THRESHOLD 100 // Tröskelvärde för fall
#define MOVE_DELAY 1000 // Fördröjning innan servot återgår till ursprungsläget

ADXL362 accelerometer; // Skapa ett accelerometer-objekt
Servo myServo; // Skapa ett Servo-objekt
int currentServoAngle = 0; // Håller koll på servons aktuella vinkel (start vid 0 grader)
bool servoMoved = false; // Flagga för att kontrollera om servot har rört sig
bool movingToRight = true; // Riktningens flagga för att hålla reda på servots rörelseriktning

void setup() {
  Serial.begin(9600); // Starta seriell kommunikation
  accelerometer.begin(CS); // Initiera accelerometern
  accelerometer.beginMeasure(); // Starta mätningen
  
  myServo.attach(SERVO_PIN); // Anslut servot till pin 8
  myServo.write(currentServoAngle); // Starta servot i neutralläget (0 grader)

  // Flytta servot till 0 grader först (önskat startläge)
  myServo.write(0); // Flytta till 0 grader
  delay(MOVE_DELAY); // Vänta en specificerad tid
  
  // Flytta servot till 90 grader och vänta
  myServo.write(90); // Flytta till 90 grader
  delay(MOVE_DELAY); // Vänta en specificerad tid
}

void loop() {
  // Inhämtning av accelerometerdata
  int x, y, z, t;
  accelerometer.readXYZTData(x, y, z, t);

  // Visa rådata
  Serial.print("x = ");
  Serial.print(x);
  Serial.print("\t y = ");
  Serial.print(y);
  Serial.print("\t z = ");
  Serial.print(z);
  Serial.print("\t t = ");
  Serial.println(t);

  // Kontrollera om z-värdet indikerar ett fall och om servot ännu inte har rört sig
  if (abs(z) < FALL_THRESHOLD && !servoMoved) {
    Serial.println("Fall upptäckt! Flyttar servot...");

    if (movingToRight) {
      myServo.write(90); // Flytta till 90 grader
    } else {
      myServo.write(0); // Flytta tillbaka till 0 grader
    }
    
    delay(MOVE_DELAY); // Vänta en specificerad tid innan återgång

    // Växla riktning inför nästa rörelse
    movingToRight = !movingToRight; // Byt riktning
    servoMoved = true; // Sätt flaggan för att indikera att servot har rört sig
  }

  // Kontrollera om servot har nått målvinkeln och återställ flaggan
  if (servoMoved && (myServo.read() == (movingToRight ? 90 : 0))) {
    servoMoved = false; // Återställ flaggan för att tillåta servot att röra sig igen
  }

  // Vänta en stund innan nästa mätning
  delay(100);
}
