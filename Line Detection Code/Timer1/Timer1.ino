int readPin = A0; 
int threshhold = 900;
int anVal = 0;
bool prev = true; // The previous value of the boolean
bool curr = true; // The current value of the boolean
int sec = 0;
void setup() {
	Serial.begin(9600);   //setup serial at given baud rate
}

void loop() {
	anVal = analogRead(readPin);  //read the input pin
  curr = anVal > threshold ? true : false;
    sleep(1);
    sec++;
  if (prev != curr) {
    Serial.println("Change:",sec);
    sec = 0;
  }
  prev = curr
}