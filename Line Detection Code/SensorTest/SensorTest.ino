int readPin = A0; 

int val = 0; 
void setup() {
	Serial.begin(9600);           //setup serial at given baud rate
}

void loop() {
	val = analogRead(readPin);  //read the input pin
	Serial.println(val);          //debug value over serial
}