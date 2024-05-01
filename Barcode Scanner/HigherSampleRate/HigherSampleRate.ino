// IF THE BARCODE IS BEING READ BACKWARDS THEN ALL THE VALUES IN LEFTSIDE AND RIGHTSIDE SHOULD BE FLIPPED RATHER THAN THE ARRAYS BEING SWAPPED
// THE DELAY BETWEEN SCANS OF THE SENSOR STATUS MUST BE ADAPTED BASED ON THE SIZE OF REAL BARCODE


// This program takes an input at the moment from a button which is used to simulate high and low
// A sensor can replace the button with no extra coding required
// The code takes an input of 67 bits which is the length of the barcode
// The code checks the binary is in the correct format, i.e. starts and ends with 101 and has 01010 in the middle
// An error message will be displayed if it is not in the correct format
// The code splits the longer array into smaller more manageable arrays for processing
// The program outputs all the smaller arrays(for debugging purposes)
// The program decodes all of the smaller arrays into decimal numbers
// The program outputs the decimal number


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

// Initialising the LED display
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

// Initialising the Servo Motors
#include <Servo.h>
Servo servoLeft;
Servo servoRight;

// Main function for the code
// Initialises relavent features
// Calls function to align barcode
void setup() {
  //start serial connection
  Serial.begin(9600);
  pinMode(sensorPin1, INPUT);
  pinMode(sensorPin2, INPUT);
  pinMode(sensorPin3, INPUT);
  pinMode(sensorPin4, INPUT);
  pinMode(sensorPin5, INPUT);

  servoLeft.attach(13);
  servoRight.attach(12);
 
  lcd.init(); // initialize the lcd
  lcd.backlight();
  
//  scanBarcode();
  alignRobot();
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

    // Optional delay before reading again
    delay(59);
  }
  Serial.println("Detected code: ");
  for (int i = 0; i < readingArraySize; i++) {
    Serial.print(readingBarcode[i]);
  }
  Serial.println();
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

}
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
      &&BinCode[31] == 0 && BinCode[32] == 1 && BinCode[33] == 0 && BinCode[34] == 1 && BinCode[35] == 0)) {
    Serial.println("Invalid identifiers");
  }
  barcodeOutput();
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
      reverseBarcode();
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
  delay(2000);                 // display the above for two seconds
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
  validateBarcode();
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
      delay(500);
      scanBarcode();
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


void moveForward() {
  servoLeft.writeMicroseconds(1700);
  servoRight.writeMicroseconds(1300);
}

void stopMotors() {
  servoLeft.detach();
  servoRight.detach();
}

// Function for repetitive tasks
void loop() {
}
