int readPin = A3; 

int val = 0; 
void setup() {
	Serial.begin(9600);           //  setup serial
}

void loop() {
	val = analogRead(readPin);  // read the input pin
	Serial.println(val);          // debug value
}