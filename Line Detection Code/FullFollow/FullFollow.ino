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
    updateSensors();

    servoLeft.writeMicroseconds(1555);  // left wheel forwards (calibrated straight speed for accuracy)
    servoRight.writeMicroseconds(1430); // right wheel forwards (calibrated straight speed for accuracy)

    if(centre && !farLeft && !farRight){
      zone = 2;
    }else if (!centre && !left && !right && !farLeft && !farRight){
      // crossover, so do nothing
    }else if (!left){
      do{
        updateSensors();
        servoLeft.writeMicroseconds(1490);  // left wheel stop
        servoRight.writeMicroseconds(1450); // right wheel slow forward
      }while(!left);
    }else if (!right){
      do{
        updateSensors();
        servoLeft.writeMicroseconds(1525); // left wheel slow forward
        servoRight.writeMicroseconds(1500); // right wheel stop
      }while(!right);
    }


  }while(zone = 1);

  // white on black following code
  do{ 
    updateSensors();

    servoLeft.writeMicroseconds(1555);  // left wheel forwards (calibrated straight speed for accuracy)
    servoRight.writeMicroseconds(1430); // right wheel forwards (calibrated straight speed for accuracy)
    if (!centre && farLeft && farRight){
      zone = 3;
    }else if (centre && left && right && farLeft && farRight){
      // crossover, so do nothing
    }else if (!right){
      do{
        updateSensors();
        servoLeft.writeMicroseconds(1490);  // left wheel stop
        servoRight.writeMicroseconds(1450); // right wheel slow forward
      }while(!right);
    }else if (!left){
      do{
        updateSensors();
        servoLeft.writeMicroseconds(1525); // left wheel slow forward
        servoRight.writeMicroseconds(1500); // right wheel stop
      }while(!left);
    }
    

  }while(zone = 2);

  // black on white following code
  do{ 
    updateSensors();

    servoLeft.writeMicroseconds(1555);  // left wheel forwards (calibrated straight speed for accuracy)
    servoRight.writeMicroseconds(1430); // right wheel forwards (calibrated straight speed for accuracy)

    if(centre && farLeft && farRight){
      zone = 4;
    }else if(!left){
      do{
        updateSensors();
        servoLeft.writeMicroseconds(1490);  // left wheel stop
        servoRight.writeMicroseconds(1450); // right wheel slow forward
      }while(!left);
    }else if (!right){
      do{
        updateSensors();
        servoLeft.writeMicroseconds(1525); // left wheel slow forward
        servoRight.writeMicroseconds(1500); // right wheel stop
      }while(!right);
    }

  }while(zone = 3);


  // barcode scanning code
  do{ 
    updateSensors();

    //read barcode



  }while(zone = 4);

  // drive to stop position
  do{ 
    updateSensors();
    if(!farLeft && !farRight){
      zone = 6
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
  right = digitalRead(3);
  farRight = digitalRead(2);  
}
