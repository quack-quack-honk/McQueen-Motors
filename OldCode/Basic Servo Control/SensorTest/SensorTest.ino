const int photodiodePin = 2;    // Digital pin where the photodiode is connected
const int threshold = 500;      // Adjust this threshold based on your environment

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  Serial.println("Photodiode Sensor Test:");
  pinMode(photodiodePin, INPUT);  // Set the photodiode pin as input
}

void loop() {
  int sensorValue = digitalRead(photodiodePin);  // Read the digital value from the photodiode
  Serial.print("Sensor Value: ");
  Serial.println(sensorValue);  // Print the sensor value to the serial monitor

  // Determine whether the output is black or white based on the threshold
  if (sensorValue == LOW) {
    Serial.println("Result: Black");
  } else {
    Serial.println("Result: White");
  }

  delay(1000);  // Delay for 1 second before reading again (adjust as needed)
}
