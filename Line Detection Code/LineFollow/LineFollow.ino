#include <Servo.h>                    // Include servo library
 
Servo servoLeft;                      // Declare left and right servos
Servo servoRight;
int threshhold = 900;
bool Left, Centre, Right;

void setup() {
  servoLeft.attach(13);
  servoRight.attach(12);
	Serial.begin(9600);   //setup serial at given baud rate
}

void loop() {
  Right = (analogRead(1) > threshhold);
  Left = (analogRead(0) > threshhold); // True when left sensor is off the line

  servoLeft.writeMicroseconds(1530);  // Left wheel counterclockwise
  servoRight.writeMicroseconds(1470); // Right wheel counterclockwise


  if (!Left){
    nudgeLeft();
  }
  if (Left){
    nudgeRight();
  }


}

void nudgeLeft(){
  servoLeft.writeMicroseconds(1500);  // Left wheel stop
  servoRight.writeMicroseconds(1490); // Right wheel slow forwards
  delay(500);
}

void nudgeRight(){
  servoLeft.writeMicroseconds(1510);  // Left wheel slow forwards
  servoRight.writeMicroseconds(1500); // Right wheel stop
  delay(500);
}