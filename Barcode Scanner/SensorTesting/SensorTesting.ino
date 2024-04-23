const int photodiodePin1 = 2;    // Digital pin where the photodiode is connected
const int photodiodePin2 = 3;    // Digital pin where the photodiode is connected
const int photodiodePin3 = 4;    // Digital pin where the photodiode is connected
const int photodiodePin4 = 5;    // Digital pin where the photodiode is connected
const int photodiodePin5 = 6;    // Digital pin where the photodiode is connected

const int threshold = 800;      // Adjust this threshold based on your environment

void setup() {
  Serial.begin(9600);  // Initialize serial communication
  pinMode(photodiodePin1, INPUT);  // Set the photodiode pin as input
  pinMode(photodiodePin2, INPUT);  // Set the photodiode pin as input
  pinMode(photodiodePin3, INPUT);  // Set the photodiode pin as input
  pinMode(photodiodePin4, INPUT);  // Set the photodiode pin as input
  pinMode(photodiodePin5, INPUT);  // Set the photodiode pin as input

}

void loop() {
  int sensorValue1 = digitalRead(photodiodePin1);  // Read the digital value from the photodiode
  int sensorValue2 = digitalRead(photodiodePin2);  // Read the digital value from the photodiode
  int sensorValue3 = digitalRead(photodiodePin3);  // Read the digital value from the photodiode
  int sensorValue4 = digitalRead(photodiodePin4);  // Read the digital value from the photodiode
  int sensorValue5 = digitalRead(photodiodePin5);  // Read the digital value from the photodiode

  Serial.print("Sensor Value1: ");
  Serial.println(sensorValue1);  // Print the sensor value to the serial monitor
  Serial.print("Sensor Value2: ");
  Serial.println(sensorValue2);  // Print the sensor value to the serial monitor
  Serial.print("Sensor Value3: ");
  Serial.println(sensorValue3);  // Print the sensor value to the serial monitor
  Serial.print("Sensor Value4: ");
  Serial.println(sensorValue4);  // Print the sensor value to the serial monitor
  Serial.print("Sensor Value5: ");
  Serial.println(sensorValue5);  // Print the sensor value to the serial monitor
  Serial.print("\n");



  delay(1000);  // Delay for 1 second before reading again
}
