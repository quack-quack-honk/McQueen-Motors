const int sensorPin1 = 2;
const int sensorPin2 = 3;
const int sensorPin3 = 4;
const int sensorPin4 = 5;
const int sensorPin5 = 6;
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
  //alignRobot();
  moveForward();
}

void alignRobot() {
  while (true) {
    int sensorValue1 = digitalRead(sensorPin1);
    int sensorValue5 = digitalRead(sensorPin5);

    Serial.print("Sensor Value1: ");
    Serial.println(sensorValue1);
    Serial.print("Sensor Value5: ");
    Serial.println(sensorValue5);
    
    if (sensorValue1 && sensorValue5){
      moveForward();
    }
    else if (sensorValue1 == LOW && sensorValue5 == LOW) {
      stopMotors();
      break;
    } else if (sensorValue1 && !sensorValue5){
        leftPivot();
        // create code so that robot pivots around the sensor
    } else if (sensorValue5 && !sensorValue1){
        rightPivot();
        // create code so that robot pivots around the sensor
    }
  }
}

void leftPivot() {
  int sensorValue1 = digitalRead(sensorPin1);
  int sensorValue5 = digitalRead(sensorPin5);
  while (sensorValue1 == LOW && sensorValue5 == HIGH) {
    int sensorValue1 = digitalRead(sensorPin1);
    int sensorValue5 = digitalRead(sensorPin5);
    curveLeft();
    delay(500);

}
  moveBackward();
  delay(500);
  alignRobot();
}

void rightPivot() {
  int sensorValue1 = digitalRead(sensorPin1);
  int sensorValue5 = digitalRead(sensorPin5);

  while (sensorValue1 == HIGH && sensorValue5 == LOW) {
    int sensorValue1 = digitalRead(sensorPin1);
    int sensorValue5 = digitalRead(sensorPin5);
    curveRight();
    delay(500);

}
  moveBackward();
  delay(500);
  alignRobot();
}

void edgeForward() {
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1485);
}

void moveForward() {
  servoLeft.writeMicroseconds(1555);  // left wheel forwards
  servoRight.writeMicroseconds(1420); // right wheel forwards
}

void curveLeft() {
  servoLeft.writeMicroseconds(1490);  // left wheel stop
  servoRight.writeMicroseconds(1480); // right wheel slow forward
}

void curveRight() {
  servoLeft.writeMicroseconds(1505); // left wheel slow forward
  servoRight.writeMicroseconds(1495); // right wheel stop
}

/*
void turnLeft() {
  servoLeft.writeMicroseconds(1300);  // Left wheel clockwise
  servoRight.writeMicroseconds(1300); // Right wheel clockwise
}

void turnRight() {
  servoLeft.writeMicroseconds(1700);  // Left wheel counterclockwise
  servoRight.writeMicroseconds(1700); // Right wheel counterclockwise
}
*/

void moveBackward() {
  servoLeft.writeMicroseconds(1475);  // Left wheel clockwise
  servoRight.writeMicroseconds(1517); // Right wheel counterclockwise
}

void stopMotors() {
  servoLeft.writeMicroseconds(1490);  // Left wheel clockwise
  servoRight.writeMicroseconds(1500); // Right wheel clockwise
}

void loop() {
}
