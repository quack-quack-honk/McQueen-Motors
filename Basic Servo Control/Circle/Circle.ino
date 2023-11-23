// Robotics with the BOE Shield - Chapter 4, project 2 - Circle
// BOE Shield-Bot navigates a circle of  1 yard diameter.

#include <Servo.h>                    // Include servo library
 
Servo servoLeft;                      // Declare left and right servos
Servo servoRight;
 
void setup()                          // Built-in initialization block
{ 
  tone(4, 3000, 1000);                // Play tone for 1 second
  delay(1000);                        // Delay to finish tone

  servoLeft.attach(13);               // Attach left signal to pin 13 
  servoRight.attach(12);              // Attach right signal to pin 12

  // Arc to the right
  servoLeft.writeMicroseconds(1700);  // Left wheel counterclockwise
  servoRight.writeMicroseconds(1700); // Right wheel clockwise slower
  delay(10000);                       // ...for 25.5 seconds
  
  servoLeft.detach();                 // Stop sending servo signals
  servoRight.detach(); 
}  
 
void loop()                          // Main loop auto-repeats
{                                    // Nothing needs repeating
}

