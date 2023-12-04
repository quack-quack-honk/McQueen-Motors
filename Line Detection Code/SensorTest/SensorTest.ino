int threshhold = 900;
int anVal = 0;
int sens1 = 0;
int sens2 = 0;
int sens3 = 0;

void setup() {
	Serial.begin(9600);   //setup serial at given baud rate
}

void loop() {
  sens1 = analogRead(0);
  sens2 = analogRead(1);
  sens3 = analogRead(2);

  Serial.print(sens1);
  Serial.print(" - ");
  Serial.print(sens2);
  Serial.print(" - ");
  Serial.print(sens3);
  Serial.print("\n");

}