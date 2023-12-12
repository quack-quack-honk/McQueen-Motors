// CHECKSUM NEEDS TO BE ADDED IN
// ALL THE CHECKS NEED TO BE ADDED IN 
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
const int buttonPin = 2;  // Pin for the sensor, change it to pin that sensor is connected to 
const int ledPin = 13;    // Pin for the inbuild Arduinio LED, no longer being used in the code
const int arraySize = 67; // Defining how long the barcode will be
bool BinCode[arraySize];  // Array containing the whole binary sequence for the barcode
bool SubArray1[28];       // Splits the first half of BinCode, removing identifier bits
bool SubArray2[28];       // Splits the second half of BinCode, removing identifier bits
bool LeftSide[28];        // Checks which direction barcode is being read from //AND NEEDS TO BE MODIFIED TO FLIP DATA IF NECESSARY
bool RightSide[28];       // Checks which direction barcode is being read from //AND NEEDS TO BE MODIFIED TO FLIP DATA IF NECESSARY
bool DBit1[7];            // Contains binary information for the 1st denary bit
bool DBit2[7];            // Contains binary information for the 2nd denary bit
bool DBit3[7];            // Contains binary information for the 3rd denary bit
bool DBit4[7];            // Contains binary information for the 4th denary bit
bool DBit5[7];            // Contains binary information for the 5th denary bit
bool DBit6[7];            // Contains binary information for the 6th denary bit
bool DBit7[7];            // Contains binary information for the 7th denary bit
bool DBit8[7];            // Contains binary information for the 8th denary bit //RESEARCH ON CHECKSUM AND MAKE SURE THIS IS DONE CORRECTLY



// Function to collect data from sensor and sort into arrays accordingly 
// Takes data inputted from sensor/button and creates an array
// 2 new arrays created to separate the data bits
// If the barcode is the opposite way around then the data should be sorted accordingly into right and left
void scanBarcode() {
  // Output the current value of the sensor every second
  for (int i = 0; i < arraySize; i++) {
    // Read and print the current sensor value
    int sensorValue = digitalRead(buttonPin);
    Serial.print("Current Sensor Status: ");
    Serial.println(sensorValue);

    // Add the button status to the array
    BinCode[i] = !sensorValue; // Invert the value for active LOW
    delay(1000);  // Wait for 1 second
  }

  // Check the first three digits of the array
  if (!(BinCode[0] && !BinCode[1] && BinCode[2])) {
    Serial.println("Invalid Barcode");
    delay(2000);  // Wait for 2 seconds
    // Clear the array
    memset(BinCode, 0, sizeof(BinCode));
    scanBarcode();  // Call the function recursively for a new scan
  }

  // Split BinCode into two subarrays
  memcpy(SubArray1, BinCode + 3, 28 * sizeof(bool));
  memcpy(SubArray2, BinCode + 36, 28 * sizeof(bool));  // Modified starting index

  // Check and assign subarrays to LeftSide and RightSide
  if (SubArray1[0] == 0 && SubArray2[0] == 1) {
    memcpy(LeftSide, SubArray1, 28 * sizeof(bool));
    memcpy(RightSide, SubArray2, 28 * sizeof(bool));
  } else if (SubArray2[0] == 0 && SubArray1[0] == 1) {
    memcpy(LeftSide, SubArray2, 28 * sizeof(bool));
    memcpy(RightSide, SubArray1, 28 * sizeof(bool));
  } else {
    Serial.println("Invalid Subarrays");
    // You may add additional logic or handling for invalid cases
    memset(BinCode, 0, sizeof(BinCode));
    scanBarcode();  // Call the function recursively for a new scan
  }
}



// Function for the decoding of the binary information into 8-bit denary
// Encoding key is defined
// Each smaller 'DBit' array is decoded using the key
void decodeBarcode() {
  // Define the encoding mappings for DBit1 to DBit4
  const char* encoding1[] = {"0001101", "0011001", "0010011", "0111101", "0100011", "0110001", "0101111", "0111011", "0110111", "0001011"};

  // Define the encoding mappings for DBit5 to DBit8
  const char* encoding2[] = {"1110010", "1100110", "1101100", "1000010", "1011100", "1001110", "1010000", "1000100", "1001000", "1110100"};

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
    } else if (i == 2){
      digit = decodeDigit(DBit2 + (i - 2) * 7, encoding1, 7);
    } else if (i == 3){
      digit = decodeDigit(DBit3 + (i - 3) * 7, encoding1, 7);
    } else if (i == 4){
      digit = decodeDigit(DBit4 + (i - 4) * 7, encoding1, 7);
    } else if (i == 5){
      digit = decodeDigit(DBit5 + (i - 5) * 7, encoding2, 7);
    } else if (i == 6){
      digit = decodeDigit(DBit6 + (i - 6) * 7, encoding2, 7);
    } else if (i == 7){
      digit = decodeDigit(DBit7 + (i - 7) * 7, encoding2, 7);
    } else {
      digit = decodeDigit(DBit8 + (i - 8) * 7, encoding2, 7);
    }

    if (digit != -1) {
      decodedDigits[i - 1] = digit;
    } else {
      decodedDigits[i - 1] = -1;  // Indicates an error
    }
  }

  // Print the array of decoded digits
  Serial.print("Decoded Digits: ");
  for (int i = 0; i < 8; i++) {
    Serial.print(decodedDigits[i]);
    Serial.print(" ");
  }
  Serial.println();
}



// Main function for the code
// Initialises relavent features
// Calls function to scan the barcode and create arrays
// Prints all relavent arrays
// Calls function to decode the relavent arrays
void setup() {
  //start serial connection
  Serial.begin(9600);
  //configure pin 2 as an input and enable the internal pull-up resistor
  pinMode(2, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP); // Use INPUT_PULLUP for active LOW button
  pinMode(ledPin, OUTPUT);

  // Perform the barcode scanning
  scanBarcode();

  // Print the array values to the Serial Monitor at the end
  Serial.println("Binary Barcode: ");
  for (int i = 0; i < arraySize; i++) {
    Serial.print(BinCode[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Print the first subarray
  Serial.println("SubArray1: ");
  for (int i = 0; i < 28; i++) {
    Serial.print(SubArray1[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Print the second subarray
  Serial.println("SubArray2: ");
  for (int i = 0; i < 28; i++) {
    Serial.print(SubArray2[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Check and assign subarrays to LeftSide and RightSide
  // IF SUBARRAY1 IS BEING ASSIGNED TO THE RIGHT SIDE THEN ALL ARRAYS NEED TO BE FLIPPED
  if (SubArray1[0] == 0 && SubArray2[0] == 1) {
    memcpy(LeftSide, SubArray1, 28 * sizeof(bool));
    memcpy(RightSide, SubArray2, 28 * sizeof(bool));
  } else if (SubArray2[0] == 0 && SubArray1[0] == 1) {
    memcpy(LeftSide, SubArray2, 28 * sizeof(bool));
    memcpy(RightSide, SubArray1, 28 * sizeof(bool));
  } else {
    Serial.println("Invalid Subarrays");
    // You may add additional logic or handling for invalid cases
    memset(BinCode, 0, sizeof(BinCode));
    scanBarcode();  // Call the function recursively for a new scan
  }

  // Print the LeftSide and RightSide arrays
  Serial.println("LeftSide: ");
  for (int i = 0; i < 28; i++) {
    Serial.print(LeftSide[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Create and print DBit1 array
  Serial.println("DBit1: ");
  memcpy(DBit1, LeftSide, 7 * sizeof(bool));
  for (int i = 0; i < 7; i++) {
    Serial.print(DBit1[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Create and print DBit2 array
  Serial.println("DBit2: ");
  memcpy(DBit2, LeftSide + 7, 7 * sizeof(bool));
  for (int i = 0; i < 7; i++) {
    Serial.print(DBit2[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Create and print DBit3 array
  Serial.println("DBit3: ");
  memcpy(DBit3, LeftSide + 14, 7 * sizeof(bool));
  for (int i = 0; i < 7; i++) {
    Serial.print(DBit3[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Create and print DBit4 array
  Serial.println("DBit4: ");
  memcpy(DBit4, LeftSide + 21, 7 * sizeof(bool));
  for (int i = 0; i < 7; i++) {
    Serial.print(DBit4[i]);
    Serial.print(" ");
  }
  Serial.println();

  Serial.println("RightSide: ");
  for (int i = 0; i < 28; i++) {
    Serial.print(RightSide[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Create and print DBit5 array
  Serial.println("DBit5: ");
  memcpy(DBit5, RightSide, 7 * sizeof(bool));
  for (int i = 0; i < 7; i++) {
    Serial.print(DBit5[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Create and print DBit6 array
  Serial.println("DBit6: ");
  memcpy(DBit6, RightSide + 7, 7 * sizeof(bool));
  for (int i = 0; i < 7; i++) {
    Serial.print(DBit6[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Create and print DBit7 array
  Serial.println("DBit7: ");
  memcpy(DBit7, RightSide + 14, 7 * sizeof(bool));
  for (int i = 0; i < 7; i++) {
    Serial.print(DBit7[i]);
    Serial.print(" ");
  }
  Serial.println();

  // Create and print DBit8 array
  Serial.println("DBit8: ");
  memcpy(DBit8, RightSide + 21, 7 * sizeof(bool));
  for (int i = 0; i < 7; i++) {
    Serial.print(DBit8[i]);
    Serial.print(" ");
  }
  Serial.println();

  decodeBarcode();
}


// Function for repetitive tasks
void loop() {
  // No need for loop logic since scanning is performed in the setup
}