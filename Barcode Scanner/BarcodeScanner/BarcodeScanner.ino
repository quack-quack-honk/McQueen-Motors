// CHECKSUM NEEDS TO BE ADDED IN
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
const int photodiodePin = 2;    // Digital pin where the photodiode is connected
const int threshold = 500;      // Adjust this threshold based on your environment

const int buttonPin = 2;   // Pin for the sensor, change it to pin that sensor is connected to
const int ledPin = 13;     // Pin for the inbuild Arduinio LED, no longer being used in the code
const int arraySize = 67;  // Defining how long the barcode will be
bool BinCode[arraySize];   // Array containing the whole binary sequence for the barcode
bool SubArray1[28];        // Splits the first half of BinCode, removing identifier bits
bool SubArray2[28];        // Splits the second half of BinCode, removing identifier bits
bool LeftSide[28];         // Checks which direction barcode is being read from //AND NEEDS TO BE MODIFIED TO FLIP DATA IF NECESSARY
bool RightSide[28];        // Checks which direction barcode is being read from //AND NEEDS TO BE MODIFIED TO FLIP DATA IF NECESSARY
bool DBit1[7];             // Contains binary information for the 1st denary bit
bool DBit2[7];             // Contains binary information for the 2nd denary bit
bool DBit3[7];             // Contains binary information for the 3rd denary bit
bool DBit4[7];             // Contains binary information for the 4th denary bit
bool DBit5[7];             // Contains binary information for the 5th denary bit
bool DBit6[7];             // Contains binary information for the 6th denary bit
bool DBit7[7];             // Contains binary information for the 7th denary bit
bool DBit8[7];             // Contains binary information for the 8th denary bit //RESEARCH ON CHECKSUM AND MAKE SURE THIS IS DONE CORRECTLY

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows


// Function to collect data from sensor and sort into arrays accordingly
// Takes data inputted from sensor/button and creates an array
// 2 new arrays created to separate the data bits
// If the barcode is the opposite way around then the data should be sorted accordingly into right and left
void scanBarcode() {
  for (int i = 0; i < arraySize; i++) {
    // Read and print the current sensor input value
    int sensorValue = digitalRead(photodiodePin);
    Serial.print("Sensor Value: ");
    Serial.println(sensorValue);

    // Add the sensor input status to the array
    BinCode[i] = sensorValue;

    // Optional delay before reading again
    delay(1000);
  }

  // Output the current value of the serial input
//  for (int i = 0; i < arraySize; i++) {
//    // Read and print the current serial input value
//    while (!Serial.available()) {
//      // Wait for serial input
//    }
//    int serialValue = Serial.read() - '0'; // Convert ASCII to integer
//    Serial.print("Current Serial Input: ");
//    Serial.println(serialValue);
//
//    // Add the serial input status to the array
//    BinCode[i] = serialValue;
//  }



  // Check the first three digits of the array
  if (!(BinCode[0] == 1 && BinCode[1] == 0 && BinCode[2] == 1
      &&BinCode[64] == 1 && BinCode[65] == 0 && BinCode[66] == 1
      &&BinCode[31] == 0 && BinCode[32] == 1 && BinCode[33] == 0 && BinCode[34] == 1 && BinCode[35] == 0)) {
    Serial.println("Invalid identifiers");
    delay(2000);  // Wait for 2 seconds
    // Clear the array
    memset(BinCode, 0, sizeof(BinCode));
    scanBarcode();  // Call the function recursively for a new scan
  }

  // Split BinCode into two subarrays
  memcpy(SubArray1, BinCode + 3, 28 * sizeof(bool));
  memcpy(SubArray2, BinCode + 36, 28 * sizeof(bool));  // Modified starting index



  // Check and assign subarrays to LeftSide and RightSide
  if (SubArray1[0], SubArray1[7], SubArray1[14], SubArray1[21] == 0 && SubArray2[0], SubArray2[7], SubArray2[14], SubArray2[21] == 1) {
    memcpy(LeftSide, SubArray1, 28 * sizeof(bool));
    memcpy(RightSide, SubArray2, 28 * sizeof(bool));
  } else if (SubArray2[0], SubArray2[7], SubArray2[14], SubArray2[21] == 0 && SubArray1[0], SubArray1[7], SubArray1[14], SubArray1[21] == 1) {
    // Reverse BinCode before copying
    reverseArray(BinCode + 3, 23);
    reverseArray(BinCode + 31, 31);

    // Copy reversed sections to LeftSide and RightSide
    memcpy(LeftSide, BinCode + 3, 28 * sizeof(bool));
    memcpy(RightSide, BinCode + 31, 28 * sizeof(bool));
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
lcd.setCursor(2, 1);         // move cursor to   (2, 1)
lcd.print(concatenatedDigits); // print message at (2, 1)
delay(2000);                 // display the above for two seconds

}




// Function to reverse the given array
void reverseArray(bool* arr, int size) {
  for (int i = 0; i < size / 2; i++) {
    bool temp = arr[i];
    arr[i] = arr[size - i - 1];
    arr[size - i - 1] = temp;
  }
}




// Main function for the code
// Initialises relavent features
// Calls function to scan the barcode and create arrays
// Prints all relavent arrays
// Calls function to decode the relavent arrays
void setup() {
  //start serial connection
  Serial.begin(9600);
  pinMode(photodiodePin, INPUT);  // Set the photodiode pin as input
  //configure pin 2 as an input and enable the internal pull-up resistor
//  pinMode(2, INPUT_PULLUP);
//  pinMode(13, OUTPUT);
//  pinMode(buttonPin, INPUT_PULLUP);  // Use INPUT_PULLUP for active LOW button
  pinMode(ledPin, OUTPUT);

  lcd.init(); // initialize the lcd
  lcd.backlight();

  // Perform the barcode scanning
  scanBarcode();

  // Print the array values to the Serial Monitor at the end
  Serial.println("Binary Barcode: ");
  for (int i = 0; i < arraySize; i++) {
    Serial.print(BinCode[i]);
  }
  Serial.println();

  // Print the first subarray
  Serial.println("SubArray1: ");
  for (int i = 0; i < 28; i++) {
    Serial.print(SubArray1[i]);
  }
  Serial.println();

  // Print the second subarray
  Serial.println("SubArray2: ");
  for (int i = 0; i < 28; i++) {
    Serial.print(SubArray2[i]);
  }
  Serial.println();

  // Print the LeftSide and RightSide arrays
  Serial.println("LeftSide: ");
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


// Function for repetitive tasks
void loop() {


}

