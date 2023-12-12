const int buttonPin = 2;  // Change this to the pin where your button is connected
const int ledPin = 13;
const int arraySize = 67;
bool BinCode[arraySize];
bool SubArray1[28];
bool SubArray2[28];  // Modified size to cover BinCode[36]-BinCode[63]
bool LeftSide[28];
bool RightSide[28];
bool DBit1[7];
bool DBit2[7];
bool DBit3[7];
bool DBit4[7];
bool DBit5[7];
bool DBit6[7];
bool DBit7[7];
bool DBit8[7];


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

  // for (int i = 0; i < 4; i++){
  //   int digitL = -1;
  //   int digitR = -1;
  //   bool valL[7];
  //   bool valR[7];

  //   for (int j = i; j < i + 7; j++){
  //     valL[j] = LeftSide[j];
  //   }

  //   for (int j = i; j < i + 7; j++){
  //     valR[j] = RightSide[j];
  //   }
    
    
  //   for (int j = 0; j < 10; j++) {
  //     bool match = true;

  //     for (int k = 0; k < 7; k++) {
  //         // Check if the boolean array matches the corresponding part of the character array
  //         if (valL[k] != (encoding1[j * 7 + k] == '1')) {
  //             match = false;
  //             break;
  //         }
  //     }

  //     if (match) {
  //         digitL = j;
  //         break;
  //     }
  //   }


  //   for (int j = 0; j < 10; j++) {
  //     bool match = true;

  //     for (int k = 0; k < 7; k++) {
  //         // Check if the boolean array matches the corresponding part of the character array
  //         if (valR[k] != (encoding2[j * 7 + k] == '1')) {
  //             match = false;
  //             break;
  //         }
  //     }

  //     if (match) {
  //         digitR = j;
  //         break;
  //     }
  //   }

  //   decodedDigits[i] = digitL;
  //   decodedDigits[i+4] = digitR;    
  // }


  // Print the array of decoded digits
  Serial.print("Decoded Digits: ");
  for (int i = 0; i < 8; i++) {
    Serial.print(decodedDigits[i]);
    Serial.print(" ");
  }
  Serial.println();
}




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

  // Print the subarrays
  Serial.println("SubArray1: ");
  for (int i = 0; i < 28; i++) {
    Serial.print(SubArray1[i]);
    Serial.print(" ");
  }
  Serial.println();

  Serial.println("SubArray2: ");
  for (int i = 0; i < 28; i++) {
    Serial.print(SubArray2[i]);
    Serial.print(" ");
  }
  Serial.println();

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

void loop() {
  // No need for loop logic since scanning is performed in the setup
}