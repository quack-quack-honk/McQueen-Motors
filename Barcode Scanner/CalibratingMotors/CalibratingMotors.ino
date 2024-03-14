const int sensorPin1 = 1;
const int sensorPin2 = 2;
const int sensorPin3 = 3;
const int sensorPin4 = 4;
const int sensorPin5 = 5;
const int threshold = 500;
#include <Servo.h>
Servo servoLeft;
Servo servoRight;

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(sensorPin3, INPUT);
  pinMode(sensorPin4, INPUT);
  pinMode(sensorPin5, INPUT);
  servoLeft.attach(13);
  servoRight.attach(12);
  moveForward();
  delay(10000);
  stopMotors();
}



void moveForward() {
  servoLeft.writeMicroseconds(1600);
  servoRight.writeMicroseconds(1300);
}

void turnLeft() {
  servoLeft.writeMicroseconds(1300);  // Left wheel clockwise
  servoRight.writeMicroseconds(1300); // Right wheel clockwise
}

void curveLeft() {
  servoLeft.writeMicroseconds(1700);  // Left wheel clockwise
  servoRight.writeMicroseconds(1450); // Right wheel clockwise
}

void curveRight() {
  servoLeft.writeMicroseconds(1550);  // Left wheel clockwise
  servoRight.writeMicroseconds(1300); // Right wheel clockwise
}

void turnRight() {
  servoLeft.writeMicroseconds(1700);  // Left wheel counterclockwise
  servoRight.writeMicroseconds(1700); // Right wheel counterclockwise
}

void moveBackward() {
  servoLeft.writeMicroseconds(1300);  // Left wheel clockwise
  servoRight.writeMicroseconds(1700); // Right wheel counterclockwise
}

void stopMotors() {
  servoLeft.detach();
  servoRight.detach();
}

void loop() {
}
