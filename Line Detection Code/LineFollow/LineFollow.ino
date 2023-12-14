#include <Servo.h>                    // Include servo library
 
Servo servoLeft;                      // Declare left and right servos
Servo servoRight;
int threshhold = 900;
bool Left, Centre, Right;

void setup() {
  servoLeft.attach(13);
  servoRight.attach(12);
	Serial.begin(9600);   //setup serial at given baud rate
  servoLeft.writeMicroseconds(1570);  // Left wheel counterclockwise
  servoRight.writeMicroseconds(1430); // Right wheel counterclockwise
}

void loop() {
  Right = (analogRead(1) > threshhold);
  Left = (analogRead(0) > threshhold); // True when left sensor is off the line
  servoLeft.writeMicroseconds(1570);  // Left wheel counterclockwise
  servoRight.writeMicroseconds(1430); // Right wheel counterclockwise

  do{
    delay(1);
    Right = (analogRead(1) > threshhold);
    Left = (analogRead(0) > threshhold);
    Serial.println("Looping Forwards");
  }while(!Left & !Right);


  if (Right){
    nudgeLeft();
  }
  if (Left){
    nudgeRight();
  }

}

void nudgeLeft(){
  servoLeft.writeMicroseconds(1495);  // Left wheel stop
  servoRight.writeMicroseconds(1480); // Right wheel slow forwards
  delay(250);
  do{
    delay(1);
    Right = (analogRead(1) > threshhold);
    Left = (analogRead(0) > threshhold);
    Serial.println("Looping Left");
  }while(Right);
}

void nudgeRight(){
  servoLeft.writeMicroseconds(1510);  // Left wheel slow forwards
  servoRight.writeMicroseconds(1500); // Right wheel stop
  delay(250);
  do{
    delay(1);
    Right = (analogRead(1) > threshhold);
    Left = (analogRead(0) > threshhold);
    Serial.println("Looping Right");
  }while(Left);
}