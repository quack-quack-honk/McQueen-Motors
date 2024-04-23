const int sensorPin1 = 1;
const int sensorPin2 = 2;
const int sensorPin3 = 3;
const int sensorPin4 = 4;
const int sensorPin5 = 5;
const int threshold = 800;
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
  alignRobot();
}

  int sensorValue1 = digitalRead(sensorPin1);
  int sensorValue5 = digitalRead(sensorPin5);

void alignRobot() {
  while (true) {
    Serial.print("Sensor Value1: ");
    Serial.println(sensorValue1);
    Serial.print("Sensor Value5: ");
    Serial.println(sensorValue5);
    
    while(sensorValue1 == HIGH && sensorValue5 == HIGH){
      moveForward();
    }

    if (sensorValue1 == LOW && sensorValue5 == LOW) {
      stopMotors();
      break;
    } else if (sensorValue1){
        leftPivot();
        // create code so that robot pivots around the sensor
    } else if (sensorValue5){
        rightPivot();
        // create code so that robot pivots around the sensor
    }
  }
}

void leftPivot() {
  while (sensorValue1 == LOW && sensorValue5 == HIGH) {
  // modify wheel speeds so robot pivots around sensor
  }
  moveBackward();
  delay(1000);
  alignRobot();
}

void rightPivot() {
  while (sensorValue1 == HIGH && sensorValue5 == LOW) {
  // modify wheel speeds so robot pivots around sensor
  }
  moveBackward();
  delay(1000);
  alignRobot();
}




void moveForward() {
  servoLeft.writeMicroseconds(1520);
  servoRight.writeMicroseconds(1480);
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
