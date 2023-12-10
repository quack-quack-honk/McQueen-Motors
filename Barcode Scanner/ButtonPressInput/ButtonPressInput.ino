const int buttonPin = 2;  // Change this to the pin where your button is connected
const int ledPin = 13;
const int arraySize = 67;
bool BinCode[arraySize];
bool SubArray1[28]; // Array from BinCode[3] to BinCode[30]
bool SubArray2[28]; // Array from BinCode[36] to BinCode[63]

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP); // Use INPUT_PULLUP for active LOW button

  while (true) {
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

    // Print the array values to the Serial Monitor at the end
    Serial.println("Binary Barcode: ");
    for (int i = 0; i < arraySize; i++) {
      Serial.print(BinCode[i]);
      Serial.print(" ");
    }
    Serial.println();

    // Check if the array starts with "101" and break the loop if true
    if (BinCode[0] == 1 && BinCode[1] == 0 && BinCode[2] == 1) {
      Serial.println("Binary Barcode starts with '101'.");
      break;  // Exit the while loop
    } else {
      Serial.println("Binary Barcode does not start with '101'. Clearing the array and trying again.");
      memset(BinCode, 0, sizeof(BinCode)); // Clear the array
      delay(2000); // Wait for 2 seconds before retrying
    }
  }

  // Create SubArray1 from BinCode[3] to BinCode[30]
  for (int i = 0; i < 28; i++) {
    SubArray1[i] = BinCode[i + 3];
  }

  // Create SubArray2 from BinCode[36] to BinCode[63]
  for (int i = 0; i < 28; i++) {
    SubArray2[i] = BinCode[i + 36];
  }

  // Print the SubArrays to the Serial Monitor
  Serial.println("SubArray1:");
  for (int i = 0; i < 28; i++) {
    Serial.print(SubArray1[i]);
    Serial.print(" ");
  }
  Serial.println();

  Serial.println("SubArray2:");
  for (int i = 0; i < 28; i++) {
    Serial.print(SubArray2[i]);
    Serial.print(" ");
  }
  Serial.println();
}

void loop() {
  // The loop function can be left empty for this example
}
