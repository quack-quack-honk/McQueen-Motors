#include <Servo.h>                    // Include servo library
// Initialising the LED display
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

Servo servoLeft;                      // Declare left and right servos
Servo servoRight;
bool left, centre, right;
bool farLeft, farRight;

// Defining all global variables
const int sensorPin1 = 2;
const int sensorPin2 = 3;
const int sensorPin3 = 4;
const int sensorPin4 = 5;
const int sensorPin5 = 6;
const int BinCodeSize = 67;// Defining how long the barcode will be
const int sampleRate = 5;
const int readingArraySize = BinCodeSize*sampleRate;
bool BinCode[BinCodeSize]; // Array containing the whole binary sequence for the barcode
bool readingBarcode[readingArraySize];
bool LeftSide[28];         // Splits the first half of BinCode, removing identifier bits
bool RightSide[28];        // Splits the second half of BinCode, removing identifier bits
bool DBit1[7];             // Contains binary information for the 1st denary bit
bool DBit2[7];             // Contains binary information for the 2nd denary bit
bool DBit3[7];             // Contains binary information for the 3rd denary bit
bool DBit4[7];             // Contains binary information for the 4th denary bit
bool DBit5[7];             // Contains binary information for the 5th denary bit
bool DBit6[7];             // Contains binary information for the 6th denary bit
bool DBit7[7];             // Contains binary information for the 7th denary bit
bool DBit8[7];             // Contains binary information for the 8th denary bit which is also checksum bit
int zone = 1;
int n = 0; // Counter to prevent infinite looping in the case of misread
int lastState = 0;

void setup() {
  servoLeft.attach(13);
  servoRight.attach(12);
	Serial.begin(9600);   // setup serial at given baud rate
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(sensorPin3, INPUT);
  pinMode(sensorPin4, INPUT);
  pinMode(sensorPin5, INPUT);
  lcd.init(); // initialize the lcd
  lcd.backlight();
  
  // zone 1 : black line / white background
  // zone 2 : white line / black background
  // zone 3 : black line / white background
  // zone 4 : barcode
  // zone 5 : black line white background
  // zone 6 : stop position and display barcode value

  // black on white following code
  do{ 
    servoLeft.writeMicroseconds(1480 + (+40)); // 30 speed  // left wheel forwards (calibrated straight speed for accuracy)
    servoRight.writeMicroseconds(1500 - (+40)); // 30 speed // right wheel forwards (calibrated straight speed for accuracy)
    delay(100);
    updateSensors();
    Serial.println("zone 1 forwards");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Zone 1");

    if (left && right && centre){
      delay(100);
      if (left && right && centre && !farLeft && !farRight ){
        zone = 2;
      }
    }else if (right && centre && !left && farLeft && farRight){
      lastState = 1;
      do{
        updateSensors();
        servoLeft.writeMicroseconds(1490 + (-40));  // left wheel 10 backward
        servoRight.writeMicroseconds(1500 - (+40)); // right wheel 20 forward
        Serial.println("right sensor trigger");
      }while(right && centre && !left && farLeft && farRight);
    }else if (left && centre && !right && farLeft && farRight){
      lastState = 2;
      do{
        updateSensors();
        servoLeft.writeMicroseconds(1490 + (+40)); // left wheel 20 forward
        servoRight.writeMicroseconds(1500 - (-40)); // right wheel 10 backward
        Serial.println("left sensor trigger");
      }while(left && centre && !right && farLeft && farRight);
    }else if (left && !centre && !right && farLeft && farRight){
      lastState = 3;
      do{
        updateSensors();
        servoLeft.writeMicroseconds(1490 + (+30)); // left wheel 20 forward
        servoRight.writeMicroseconds(1500 - (+10)); // right wheel 10 backward
        Serial.println("left sensor trigger");
      }while(left && !centre && !right && farLeft && farRight);
    }else if (right && !centre && !left && farLeft && farRight){
      lastState = 4;
      do{
        updateSensors();
        servoLeft.writeMicroseconds(1490 + (+10));  // left wheel 10 backward
        servoRight.writeMicroseconds(1500 - (+30)); // right wheel 20 forward
        Serial.println("right sensor trigger");
      }while(right && !centre && !left && farLeft && farRight);
    }else if (right && centre && left && !farRight && farLeft){
      do{
        updateSensors();
        servoLeft.writeMicroseconds(1490 + (+30));  // left wheel 10 backward
        servoRight.writeMicroseconds(1500 - (-30)); // right wheel 20 forward
        Serial.println("right sensor trigger");
      }while(right && centre && left && !farRight && farLeft);
    }else if (right && centre && left && farRight && !farLeft){
      do{
        updateSensors();
        servoLeft.writeMicroseconds(1490 + (-30));  // left wheel 10 backward
        servoRight.writeMicroseconds(1500 - (+30)); // right wheel 20 forward
        Serial.println("right sensor trigger");
      }while(right && centre && left && farRight && !farLeft);
    }else if (!left && !centre && right && farRight && !farLeft){
      do{
        updateSensors();
        servoLeft.writeMicroseconds(1490 + (-30));  // left wheel 10 backward
        servoRight.writeMicroseconds(1500 - (+30)); // right wheel 20 forward
        Serial.println("right sensor trigger");
      }while(right && centre && left && farRight && !farLeft);
    }



  }while(zone == 1);

  // white on black following code
  do{ 
    servoLeft.writeMicroseconds(1490 + (+30)); // 30 speed  // left wheel forwards (calibrated straight speed for accuracy)
    servoRight.writeMicroseconds(1500 - (+30)); // 30 speed // right wheel forwards (calibrated straight speed for accuracy)
    delay(100);
    updateSensors();
    Serial.println("zone 2 forwards");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Zone 2");


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
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Zone 3");


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
    Serial.println("zone 4 forwards");
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Zone 4");

    updateSensors();

    alignRobot();

  }while(zone = 4);

  stopMotors();

}


// Function to drive over barcode and collect data to put into an array
// Robot drives for a specified time which corresponds to the length of the barcode
// Takes data inputted from sensor/button and creates an array
void scanBarcode() {
  lcd.clear();                 // clear display
  edgeForward();


  for (int i = 0; i < readingArraySize; i++) {
    // Read and print the current sensor input value
    int sensorValue = digitalRead(sensorPin3);
    sensorValue = sensorValue == 0 ? 1 : 0; // Flip the value
    Serial.print("Sensor Value: ");
    Serial.println(sensorValue);

    // Add the sensor input status to the array
    readingBarcode[i] = sensorValue;

    if (i == sampleRate*9) {
      realigning();   // Call an aligning function
      edgeForward(); // Resume the movement after alignment
    } else if (i == sampleRate*16) {
      realigning();   // Call an aligning function
      edgeForward(); // Resume the movement after alignment
    } else if (i == sampleRate*23) {
      realigning();   // Call an aligning function
      edgeForward(); // Resume the movement after alignment
    } else if (i == sampleRate*30) {
      realigning();   // Call an aligning function
      edgeForward(); // Resume the movement after alignment
    } else if (i == sampleRate*32) {
      realigning();   // Call an aligning function
      edgeForward(); // Resume the movement after alignment
    } else if (i == sampleRate*36) {
      realigning();   // Call an aligning function
      edgeForward(); // Resume the movement after alignment
    } else if (i == sampleRate*43) {
      realigning();   // Call an aligning function
      edgeForward(); // Resume the movement after alignment
    } else if (i == sampleRate*50) {
      realigning();   // Call an aligning function
      edgeForward(); // Resume the movement after alignment
    } else if (i == sampleRate*57) {
      realigning();   // Call an aligning function
      edgeForward(); // Resume the movement after alignment
    } 

    // Optional delay before reading again
    delay(58);
  }
  Serial.println("Detected code: ");
  for (int i = 0; i < readingArraySize; i++) {
    Serial.print(readingBarcode[i]);
  }
  Serial.println();
  delay(5000);
  stopMotors();
  createBarcode();
}


/*
  // Output the current value of the serial input
  for (int i = 0; i < readingArraySize; i++) {
    // Read and print the current serial input value
    while (!Serial.available()) {
      // Wait for serial input
    }
    int serialValue = Serial.read() - '0'; // Convert ASCII to integer
    Serial.print("Current Serial Input: ");
    Serial.println(serialValue);

    // Add the serial input status to the array
    readingBarcode[i] = serialValue;
  }
  Serial.println("testing Barcode: ");
  for (int i = 0; i < readingArraySize; i++) {
    Serial.print(readingBarcode[i]);
  }
  Serial.println();
  createBarcode();
*/

void createBarcode() {
  int i, j;
  int count_0, count_1;

    // Iterate over the data array in chunks of 10
    for (i = 0, j = 0; j < BinCodeSize; i += sampleRate, j++) {
      count_0 = 0;
      count_1 = 0;

      // Count the occurrences of 0s and 1s in the current chunk
      for (int k = 0; k < sampleRate; k++) {
        if (readingBarcode[i + k] == 0)
          count_0++;
        else
          count_1++;
        }

      // Append the more frequent number to bin_code
      if (count_0 > count_1)
        BinCode[j] = 0;
      else 
        BinCode[j] = 1;
    }
      Serial.print("BinaryCode:");
      for (int i = 0; i < BinCodeSize; i++) {
        Serial.print(BinCode[i]);
        Serial.print(" ");
      }
  validateBarcode();
}


void validateBarcode() {
  // Check the identifier digits of the array
  if (!(BinCode[0] == 1 && BinCode[1] == 0 && BinCode[2] == 1
      &&BinCode[64] == 1 && BinCode[65] == 0 && BinCode[66] == 1
      &&BinCode[31] == 0 && BinCode[32] == 1 && BinCode[33] == 0 && BinCode[34] == 1 && BinCode[35] == 0)&& (n != 2)) {
    Serial.println("Invalid identifiers");
    n = n+1;
    reverseBarcode();
  } else if ((BinCode[0] == 1 && BinCode[1] == 0 && BinCode[2] == 1
      &&BinCode[64] == 1 && BinCode[65] == 0 && BinCode[66] == 1
      &&BinCode[31] == 0 && BinCode[32] == 1 && BinCode[33] == 0 && BinCode[34] == 1 && BinCode[35] == 0)&& (n != 2)) {
    n = n+1;
    reverseBarcode();
  } else if (n == 2) {
    moveForward();
    delay(1000);
    stopMotors();
    lcd.clear();                  // clear display
    lcd.setCursor(0, 0);          // move cursor to (0, 0)
    lcd.print("Scan Failed");     // print message at (0, 0)
    delay(100000);
  }
}

// Function for the decoding of the binary information into 8-bit denary
// Encoding key is defined
// Each smaller 'DBit' array is decoded using the key
void decodeBarcode() {
  // Define the encoding mappings for DBit1 to DBit4
  const char* encoding1[] = { "0001101", "0011001", "0010011", "0111101", "0100011", "0110001", "0101111", "0111011", "0110111", "0001011" };

  // Define the encoding mappings for DBit5 to DBit8
  const char* encoding2[] = { "1110010", "1100110", "1101100", "1000010", "1011100", "1001110", "1010000", "1000100", "1001000", "1110100" };

  // Function to decode a single digit
  auto decodeDigit = [](bool* dbit, const char* encoding[], int size) -> int {
    String dbitString = "";
    for (int i = 0; i < size; i++) {
      dbitString += dbit[i] ? '1' : '0';
    }

    for (int i = 0; i < 10; i++) {
      if (dbitString.equals(encoding[i])) {
        return i;
      }
    }
    return -1;  // If no match is found
  };

  // Array to store the decoded digits
  int decodedDigits[8];

  // Decode each digit and store in the array
  for (int i = 1; i <= 8; i++) {
    int digit = -1;
    if (i == 1) {
      digit = decodeDigit(DBit1 + (i - 1) * 7, encoding1, 7);
    } else if (i == 2) {
      digit = decodeDigit(DBit2 + (i - 2) * 7, encoding1, 7);
    } else if (i == 3) {
      digit = decodeDigit(DBit3 + (i - 3) * 7, encoding1, 7);
    } else if (i == 4) {
      digit = decodeDigit(DBit4 + (i - 4) * 7, encoding1, 7);
    } else if (i == 5) {
      digit = decodeDigit(DBit5 + (i - 5) * 7, encoding2, 7);
    } else if (i == 6) {
      digit = decodeDigit(DBit6 + (i - 6) * 7, encoding2, 7);
    } else if (i == 7) {
      digit = decodeDigit(DBit7 + (i - 7) * 7, encoding2, 7);
    } else {
      digit = decodeDigit(DBit8 + (i - 8) * 7, encoding2, 7);
    }

    if (digit != -1) {
      decodedDigits[i - 1] = digit;
    } else {
      decodedDigits[i - 1] = -1;  // Indicates an error
      Serial.println("Not a valid barcode");
//      outputDigits(decodedDigits);
      validateBarcode();
      return;  // Exit the function since it's not a valid barcode
    }
  }
  // Define a String variable to store concatenated digits
  String concatenatedDigits = "";
  // Concatenate all decoded digits into the variable
  for (int i = 0; i < 8; i++) {
    concatenatedDigits += String(decodedDigits[i]);
  }
  // Print the concatenated digits
  Serial.print("Barcode Digits: ");
  Serial.println(concatenatedDigits);
  lcd.clear();                 // clear display
  lcd.setCursor(0, 0);         // move cursor to (0, 0)
  lcd.print(concatenatedDigits); // print message at (0, 0)
  lcd.setCursor(0, 1);         // move cursor to (0, 0)
  lcd.print("Checksum: OK"); // print message at (0, 0)
  zone = 5;
  delay(100000);                 // display the above for two seconds
}



void barcodeOutput() {
  // Print the array values to the Serial Monitor at the end
  Serial.println("Binary Barcode: ");
  for (int i = 0; i < BinCodeSize; i++) {
    Serial.print(BinCode[i]);
  }
  Serial.println();

  // Create and print the LeftSide array
  Serial.println("LeftSide: ");
  memcpy(LeftSide, BinCode + 3, 28 * sizeof(bool));
  for (int i = 0; i < 28; i++) {
    Serial.print(LeftSide[i]);
  }
  Serial.println();

  // Create and print DBit1 array
  Serial.println("DBit1: ");
  memcpy(DBit1, LeftSide, 7 * sizeof(bool));
  for (int i = 0; i < 7; i++) {
    Serial.print(DBit1[i]);
  }
  Serial.println();

  // Create and print DBit2 array
  Serial.println("DBit2: ");
  memcpy(DBit2, LeftSide + 7, 7 * sizeof(bool));
  for (int i = 0; i < 7; i++) {
    Serial.print(DBit2[i]);
  }
  Serial.println();

  // Create and print DBit3 array
  Serial.println("DBit3: ");
  memcpy(DBit3, LeftSide + 14, 7 * sizeof(bool));
  for (int i = 0; i < 7; i++) {
    Serial.print(DBit3[i]);
  }
  Serial.println();

  // Create and print DBit4 array
  Serial.println("DBit4: ");
  memcpy(DBit4, LeftSide + 21, 7 * sizeof(bool));
  for (int i = 0; i < 7; i++) {
    Serial.print(DBit4[i]);
  }
  Serial.println();

  Serial.println("RightSide: ");
  memcpy(RightSide, BinCode + 36, 28 * sizeof(bool));
  for (int i = 0; i < 28; i++) {
    Serial.print(RightSide[i]);
  }
  Serial.println();

  // Create and print DBit5 array
  Serial.println("DBit5: ");
  memcpy(DBit5, RightSide, 7 * sizeof(bool));
  for (int i = 0; i < 7; i++) {
    Serial.print(DBit5[i]);
  }
  Serial.println();

  // Create and print DBit6 array
  Serial.println("DBit6: ");
  memcpy(DBit6, RightSide + 7, 7 * sizeof(bool));
  for (int i = 0; i < 7; i++) {
    Serial.print(DBit6[i]);
  }
  Serial.println();

  // Create and print DBit7 array
  Serial.println("DBit7: ");
  memcpy(DBit7, RightSide + 14, 7 * sizeof(bool));
  for (int i = 0; i < 7; i++) {
    Serial.print(DBit7[i]);
  }
  Serial.println();

  // Create and print DBit8 array
  Serial.println("DBit8: ");
  memcpy(DBit8, RightSide + 21, 7 * sizeof(bool));
  for (int i = 0; i < 7; i++) {
    Serial.print(DBit8[i]);
  }
  Serial.println();
  decodeBarcode();
}

void reverseBarcode(){
  reverseArray(BinCode, 67);
    for (int i = 0; i < 67; i++) {
    Serial.print(BinCode[i]);
    Serial.print(" ");
  }
  Serial.println();
  barcodeOutput();
//  validateBarcode();
}

// Function to reverse the given array
void reverseArray(bool* arr, int size) {
  for (int i = 0; i < size / 2; i++) {
    bool temp = arr[i];
    arr[i] = arr[size - i - 1];
    arr[size - i - 1] = temp;
  }
}

void alignRobot() {
  while (true) {
    int sensorValue1 = digitalRead(sensorPin1);
    int sensorValue5 = digitalRead(sensorPin5);
    
    if (sensorValue1 && sensorValue5){
      edgeForward();
    } else if (!sensorValue1 && !sensorValue5) {
      pauseMotors();
      scanBarcode();
    } else if (sensorValue1 && !sensorValue5){
      reverseRight();
    } else if (sensorValue5 && !sensorValue1){
      reverseLeft();
    }
  }
}

void realigning() {
  while (true) {
    int sensorValue1 = digitalRead(sensorPin1);
    int sensorValue5 = digitalRead(sensorPin5);
    
    if (sensorValue1 && sensorValue5){
      edgeForward();
    } else if (!sensorValue1 && !sensorValue5) {
      return;
    } else if (sensorValue1 && !sensorValue5){
      reverseRight();
    } else if (sensorValue5 && !sensorValue1){
      reverseLeft();
    }
  }
}

void edgeForward() {
  servoLeft.writeMicroseconds(1500);
  servoRight.writeMicroseconds(1483);
}

void curveLeft() {
  servoLeft.writeMicroseconds(1490);  // left wheel stop
  servoRight.writeMicroseconds(1480); // right wheel slow forward
}

void curveRight() {
  servoLeft.writeMicroseconds(1505); // left wheel slow forward
  servoRight.writeMicroseconds(1495); // right wheel stop
}

void reverseLeft() {
  servoLeft.writeMicroseconds(1480); 
  servoRight.writeMicroseconds(1517);
}

void reverseRight() {
  servoLeft.writeMicroseconds(1470); 
  servoRight.writeMicroseconds(1510); 
}

void moveBackward() {
  servoLeft.writeMicroseconds(1475);  // Left wheel clockwise
  servoRight.writeMicroseconds(1517); // Right wheel counterclockwise
}

void pauseMotors() {
  servoLeft.writeMicroseconds(1490);  // Left wheel clockwise
  servoRight.writeMicroseconds(1500); // Right wheel clockwise
}

void fullSpeed() {
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1300);
}

void moveForward() {
  servoLeft.writeMicroseconds(1536);
  servoRight.writeMicroseconds(1450);
}

void stopMotors() {
  servoLeft.detach();
  servoRight.detach();
}

void updateSensors(){
  farLeft = digitalRead(6);
  left = digitalRead(5);
  centre = digitalRead(4);
  right = digitalRead(3);
  farRight = digitalRead(2);  
}

void loop() {
}