#include <SPI.h>
#include <ADXL362.h>
#include <Servo.h>

#define CS 10
#define SERVO_PIN 8 // Servons pin
#define FALL_THRESHOLD 300 // Tröskelvärde för fall
#define MOVE_DELAY 1000 // Fördröjning innan servot återgår till ursprungsläget

ADXL362 accelerometer; // Skapa ett accelerometer-objekt
Servo myServo; // Skapa ett Servo-objekt
int currentServoAngle = 0; // Håller koll på servons aktuella vinkel (start vid 0 grader)
 // Riktningens flagga för att hålla reda på servots rörelseriktning

void setup() {
  Serial.begin(9600); // Starta seriell kommunikation
  SPI.end();
  accelerometer.begin(CS); // Initiera accelerometern
  accelerometer.beginMeasure(); // Starta mätningen
  
  myServo.attach(SERVO_PIN); // Anslut servot till pin 8
  myServo.write(currentServoAngle); // Starta servot i neutralläget (0 grader)

  // Flytta servot till 0 grader först (önskat startläge)
  myServo.write(0); // Flytta till 0 grader
  delay(MOVE_DELAY); // Vänta en specificerad tid
  
  // Flytta servot till 90 grader och vänta
  myServo.write(120); // Flytta till 90 grader
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
  if (z < FALL_THRESHOLD) {
    Serial.println("Fall upptäckt! Startar klockan"); //Om ett acceleration upptäcks, starta en klocka.
    delay(400); // 0.32 sekunder är tiden det tar att falla 0.5 meter.
    accelerometer.readXYZTData(x, y, z, t);

    if (z < FALL_THRESHOLD) { //Efter 400ms kontrolleras det om fallet fortfarnade varar, om så utlöses fallskärmen. 
      Serial.println("Längre fall upptäck! Utlöser fallskärm");

      
      myServo.write(-20); // Flytta tillbaka till 120 grader
      
     
       // Sätt flaggan för att indikera att servot har rört sig
    }
    delay(MOVE_DELAY); // Vänta en specificerad tid innan återgång

     
  }

  // Kontrollera om servot har nått målvinkeln och återställ flaggan
  //if (servoMoved && (myServo.read() == (movingToRight ? 120 : -20))) {
   // Återställ flaggan för att tillåta servot att röra sig igen
 // }

  // Vänta en stund innan nästa mätning
  delay(100);
}