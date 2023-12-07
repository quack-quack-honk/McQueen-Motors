#include <stdio.h>

void setup() {
  Serial.begin(9600); // Set the baud rate for serial communication
}

void loop() {
  Serial.println("Enter a 95-bit binary number:");

  char input[96];
  int count = 0;

  while (count < 95) {
    while (Serial.available() == 0) {
      // Wait for the user to enter data
    }

    char c = Serial.read();

    if (c == '0' || c == '1') {
      input[count] = c;
      count++;
    else{
      Serial.println("Enter a 0 or 1");
     }
    }
  }

  input[count] = '\0'; // Null-terminate the input

  // Check if the entered number has exactly 95 bits
  if (isValidBinaryNumber(input, 95)) {
    Serial.println("Valid 95-bit binary number!");
  } else {
    Serial.println("Error: Invalid number of bits. Please enter a 95-bit binary number.");
  }
}

bool isValidBinaryNumber(const char *binaryNumber, int expectedBits) {
  int count = 0;
  while (binaryNumber[count] != '\0') {
    if (binaryNumber[count] != '0' && binaryNumber[count] != '1') {
      // Not a valid binary digit
      return false;
    }
    count++;
  }

  return count == expectedBits;
}
