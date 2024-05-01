#include <Servo.h>                    // Include servo library
 
Servo servoLeft;                      // Declare left and right servos
Servo servoRight;
bool left, centre, right;
bool farLeft, farRight;

void setup() {
  
  servoLeft.attach(13);
  servoRight.attach(12);
	Serial.begin(9600);   // setup serial at given baud rate



  int zone = 1;
  // zone 1 : black line / white background
  // zone 2 : white line / black background
  // zone 3 : black line / white background
  // zone 4 : barcode
  // zone 5 : black line white background
  // zone 6 : stop position and display barcode value

  // black on white following code
  do{ 
    servoLeft.writeMicroseconds(1490 + (+30)); // 30 speed  // left wheel forwards (calibrated straight speed for accuracy)
    servoRight.writeMicroseconds(1500 - (+30)); // 30 speed // right wheel forwards (calibrated straight speed for accuracy)
    delay(100);
    updateSensors();
    Serial.println("zone 1 forwards");

    if (left && right){
      delay(100);
      if (left && right && !farLeft && !farRight ){
        zone = 2;
      }
    }else if (right){
      do{
        updateSensors();
        servoLeft.writeMicroseconds(1490 + (-10));  // left wheel 10 backward
        servoRight.writeMicroseconds(1500 - (+20)); // right wheel 20 forward
        Serial.println("right sensor trigger");
      }while(right);
    }else if (left){
      do{
        updateSensors();
        servoLeft.writeMicroseconds(1490 + (+20)); // left wheel 20 forward
        servoRight.writeMicroseconds(1500 - (-10)); // right wheel 10 backward
        Serial.println("left sensor trigger");
      }while(left);
    }


  }while(zone == 1);

  // white on black following code
  do{ 
    servoLeft.writeMicroseconds(1490 + (+30)); // 30 speed  // left wheel forwards (calibrated straight speed for accuracy)
    servoRight.writeMicroseconds(1500 - (+30)); // 30 speed // right wheel forwards (calibrated straight speed for accuracy)
    delay(100);
    updateSensors();
    Serial.println("zone 2 forwards");

    if (!left && !right){
      delay(100);
      if (!left && !right && farLeft && farRight ){
        zone = 3;
      }
    }else if (!right){
      do{
        updateSensors();
        servoLeft.writeMicroseconds(1490 + (-10));  // left wheel 10 backward
        servoRight.writeMicroseconds(1500 - (+20)); // right wheel 20 forward
        Serial.println("right sensor trigger");
      }while(!right);
    }else if (!left){
      do{
        updateSensors();
        servoLeft.writeMicroseconds(1490 + (+20)); // left wheel 20 forward
        servoRight.writeMicroseconds(1500 - (-10)); // right wheel 10 backward
        Serial.println("left sensor trigger");
      }while(!left);
    }
    

  }while(zone == 2);

  // black on white following code
  do{ 
    servoLeft.writeMicroseconds(1490 + (+30)); // 30 speed  // left wheel forwards (calibrated straight speed for accuracy)
    servoRight.writeMicroseconds(1500 - (+30)); // 30 speed // right wheel forwards (calibrated straight speed for accuracy)
    delay(100);
    updateSensors();
    Serial.println("zone 3 forwards");

    if (left && right && farLeft && farRight ){
        zone = 4;
      
    }else if (right){
      do{
        updateSensors();
        servoLeft.writeMicroseconds(1490 + (-10));  // left wheel 10 backward
        servoRight.writeMicroseconds(1500 - (+20)); // right wheel 20 forward
        Serial.println("right sensor trigger");
      }while(right);
    }else if (left){
      do{
        updateSensors();
        servoLeft.writeMicroseconds(1490 + (+20)); // left wheel 20 forward
        servoRight.writeMicroseconds(1500 - (-10)); // right wheel 10 backward
        Serial.println("left sensor trigger");
      }while(left);
    }


  }while(zone == 3);


  // barcode scanning code
  do{ 
    updateSensors();

    //read barcode



  }while(zone = 4);

  // drive to stop position
  do{ 
    updateSensors();
    if(!farLeft && !farRight){
      zone = 6;
    }
  }while(zone = 5);

  // stop moving and display barcode value
  do{ 
  servoLeft.writeMicroseconds(1490);  // left wheel stop
  servoRight.writeMicroseconds(1500); // right wheel stop


  }while(zone = 6);

}

void loop() {
  

}

void updateSensors(){
  farLeft = digitalRead(6);
  left = digitalRead(5);
  centre = digitalRead(4);
  right = digitalRead(4);
  farRight = digitalRead(2);  
}
