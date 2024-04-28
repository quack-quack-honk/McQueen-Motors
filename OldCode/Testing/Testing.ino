#include <Servo.h>                    // Include servo library
 
Servo servoLeft;                      // Declare left and right servos
Servo servoRight;
bool left, centre, right;
bool farLeft, farRight;

// servo values:
// 1500 = stop
// 1700 = max counter clockwise
// 1300 = max clockwise 

// accurate speeds with calibration
// left = 1555
// right = 1430

// max speeds with caliubration
// left = 1600
// right = 1300

void setup() {
  
  servoLeft.attach(13);
  servoRight.attach(12);
	Serial.begin(9600);   //setup serial at given baud rate

  delay(250);
  servoLeft.writeMicroseconds(1555); //max accurate speed with calibration
  servoRight.writeMicroseconds(1430); //max accurate speed with calibration
  delay(2000);
  servoLeft.writeMicroseconds(1490);
  servoRight.writeMicroseconds(1500);
}

void loop() {
  // put your main code here, to run repeatedly:

}
