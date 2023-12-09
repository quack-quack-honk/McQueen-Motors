const int buttonPin = 2;  // Change this to the pin where your button is connected
const int ledPin = 13;
const int arraySize = 67;
bool BinCode[arraySize];

void setup() {
  Serial.begin(9600);
  pinMode(buttonPin, INPUT_PULLUP); // Use INPUT_PULLUP for active LOW button
  pinMode(ledPin, OUTPUT);


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

  // Stop the program after printing the array
  while (true) {
    // You can add any additional logic here if needed
  }
}

void loop() {

}
