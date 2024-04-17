void setup() {
  servoLeft.attach(13);
  servoRight.attach(12);
	Serial.begin(9600);   //setup serial at given baud rate
  bool left, centre, right;
  bool farLeft, farRight;


  int zone = 1;
  //zone 1 : black line / white background
  //zone 2 : white line / black background
  //zone 3 : barcode
  //zone 4 : drive to stop position
  //zone 5 : stop position and display barcode value

  //white on black following code
  do{ 
    farLeft = digitalRead(6);
    left = digitalRead(5);
    centre = digitalRead(4);
    right = digitalRead(3);
    farRight = digitalRead(2);

    servoLeft.writeMicroseconds(1570);  // Left wheel forwards
    servoRight.writeMicroseconds(1430); // Right wheel forwards



  }while(zone = 1);

  //black on white following code
  do{ 
    farLeft = digitalRead(6);
    left = digitalRead(5);
    centre = digitalRead(4);
    right = digitalRead(3);
    farRight = digitalRead(2);



  }while(zone = 2);

  //barcode scanning code
  do{ 
    farLeft = digitalRead(6);
    left = digitalRead(5);
    centre = digitalRead(4);
    right = digitalRead(3);
    farRight = digitalRead(2);


  }while(zone = 3);

  //drive to stop position
  do{ 
    farLeft = digitalRead(6);
    left = digitalRead(5);
    centre = digitalRead(4);
    right = digitalRead(3);
    farRight = digitalRead(2);



  }while(zone = 4);

  //stop moving and display barcode value
  do{ 
  servoLeft.writeMicroseconds(1500);  // left wheel stop
  servoRight.writeMicroseconds(1500); // right wheel stop



  }while(zone = 5);

}

void loop() {
  

}
