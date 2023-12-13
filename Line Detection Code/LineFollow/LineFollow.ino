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
  Left = (analogRead(0) > threshhold); // True when left sensor is off the line
  Right = (analogRead(1) > threshhold);

  servoLeft.writeMicroseconds(1700);  // Left wheel counterclockwise
  servoRight.writeMicroseconds(1300); // Right wheel counterclockwise

  if (Left && !Right){
    nudgeRight();
  }
  if (!Left && Right){
    nudgeLeft();
  }

}

void nudgeLeft(){
  servoLeft.writeMicroseconds(1500);  // Left wheel stop
  servoRight.writeMicroseconds(1200); // Right wheel slow forwards
  delay(500);
}

void nudgeRight(){
  servoLeft.writeMicroseconds(1600);  // Left wheel slow forwards
  servoRight.writeMicroseconds(1500); // Right wheel stop
  delay(500);
}