#include <Servo.h>                    // Include servo library
 
Servo servoLeft;                      // Declare left and right servos
Servo servoRight;
bool left, centre, right;
bool farLeft, farRight;

// servo values:
// 1500 = stop
// 1700 = max counter clockwise
// 1300 = max clockwise

void setup() {
  
  servoLeft.attach(13);
  servoRight.attach(12);
	Serial.begin(9600);   //setup serial at given baud rate

}

void loop() {
  // put your main code here, to run repeatedly:

}
