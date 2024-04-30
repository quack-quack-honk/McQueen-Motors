#include <Servo.h>

Servo servoLeft;
Servo servoRight;

int state = 0;  // Initial state

void setup() {
  servoLeft.attach(13);
  servoRight.attach(12);
}

void moveForward() {
  servoLeft.writeMicroseconds(1700);  // Left wheel counterclockwise
  servoRight.writeMicroseconds(1300); // Right wheel counterclockwise
}

void turnLeft() {
  servoLeft.writeMicroseconds(1300);  // Left wheel clockwise
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

void spinCircle() {
  servoLeft.writeMicroseconds(1700);  // Left wheel counterclockwise
  servoRight.writeMicroseconds(1300); // Right wheel counterclockwise slower
}

void stopMotors() {
  servoLeft.detach();
  servoRight.detach();
}

void loop() {
  switch (state) {
    case 0:  // Initial state: move forward
      moveForward();
      delay(2000);  // Move forward for 2 seconds
      state = 1;    // Next state: turn left
      break;

    case 1:  // Turn left
      turnLeft();
      delay(2000);  // Turn left for 2 seconds
      state = 2;    // Next state: turn right
      break;

    case 2:  // Turn right
      turnRight();
      delay(2000);  // Turn right for 2 seconds
      state = 3;    // Next state: move backward
      break;

    case 3:  // Move backward
      moveBackward();
      delay(2000);  // Move backward for 2 seconds
      state = 4;    // Next state: spin in a circle
      break;

    case 4:  // Spin in a circle
      spinCircle();
      delay(5000);  // Spin in a circle for 5 seconds
      stopMotors();
      state = 0;    // Reset to the initial state
      break;
  }
}
