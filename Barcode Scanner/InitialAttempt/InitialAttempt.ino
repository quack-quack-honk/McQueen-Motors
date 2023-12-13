void setup() {
  Serial.begin(9600);  // Initialize serial communication
}

void loop() {
  if (Serial.available() > 0) {
    char serialChar = Serial.read();
    Serial.print("Received: ");
    Serial.println(serialChar);
  }
}
