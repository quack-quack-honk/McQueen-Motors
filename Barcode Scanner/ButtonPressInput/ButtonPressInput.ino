const int buttonPin = 2;  // Change this to the pin where your button is connected
const int ledPin = 13;
const int arraySize = 67;
bool BinCode[arraySize];
bool SubArray1[28];
bool SubArray2[28];  // Modified size to cover BinCode[36]-BinCode[63]
bool LeftSide[28];
bool RightSide[28];

// Custom encoded key for LeftSide
const char LeftSideKey[10][7] = {
  "0001101", // 0
  "0011001", // 1
  "0010011", // 2
  "0111101", // 3
  "0100011", // 4
  "0110001", // 5
  "0101111", // 6
  "0111011", // 7
  "0110111", // 8
  "0001011"  // 9
};

// Custom encoded key for RightSide
const char RightSideKey[10][7] = {
  "1110010", // 0
  "1100110", // 1
  "1101100", // 2
  "1000010", // 3
  "1011100", // 4
  "1001110", // 5
  "1010000", // 6
  "1000100", // 7
  "1001000", // 8
  "1110100"  // 9
};

int binaryToDecimal(bool binary[7]) {
  int decimal = 0;
  for (int i = 0; i < 7; i++) {
    decimal = (decimal << 1) | binary[i];
  }
  return decimal;
}

void scanBarcode() {
  // Output the current value of the sensor every second
  for (int i = 0; i < arraySize; i++) {
    // Read and print the current sensor value
    int sensorValue = digitalRead(buttonPin);
    Serial.print("Current Sensor Status: ");
    Serial.println(sensorValue);

    // Add the button status to the array
    BinCode[i] = !sensorValue;  // Invert the value for active LOW
    delay(1000);                // Wait for 1 second
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

void setup() {
  //start serial connection
  Serial.begin(9600);
  //configure pin 2 as an input and enable the internal pull-up resistor
  pinMode(2, INPUT_PULLUP);
  pinMode(13, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);  // Use INPUT_PULLUP for active LOW button
  pinMode(ledPin, OUTPUT);

  // Perform the barcode scanning
  scanBarcode();

  // Convert binary values to decimal using the custom encoded key
  int leftSideDecimal = binaryToDecimal(LeftSide);
  int rightSideDecimal = binaryToDecimal(RightSide);

  // Print the decimal values to the Serial Monitor
  Serial.print("LeftSide Decimal: ");
  Serial.println(leftSideDecimal);
  Serial.print("RightSide Decimal: ");
  Serial.println(rightSideDecimal);

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

  Serial.println("RightSide: ");
  for (int i = 0; i < 28; i++) {
    Serial.print(RightSide[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void loop() {
  // No need for loop logic since scanning is performed in the setup
}
