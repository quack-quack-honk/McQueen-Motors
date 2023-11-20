int readPin = A0; 
int threshhold = 900;
int anVal = 0;

void setup() {
	Serial.begin(9600);   //setup serial at given baud rate
}

void loop() {
	anVal = analogRead(readPin);  //read the input pin
	if (anVal > threshhold) {
    Serial.println("Light");
  } else {
    Serial.println("Dark");
  }

}