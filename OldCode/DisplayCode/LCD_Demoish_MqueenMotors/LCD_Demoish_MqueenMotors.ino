#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // I2C address 0x27, 16 column and 2 rows

void setup()
{
  lcd.init(); // initialize the lcd
  lcd.backlight();
}

void loop()
{
  lcd.clear();                 // clear display
  lcd.setCursor(5, 0);         // move cursor to   (2, 1)
  lcd.print("KACHOW"); // print message at (2, 1)
  lcd.setCursor(1, 1);         // move cursor to   (2, 1)
  lcd.print("McQueen Motors"); // print message at (2, 1)
  delay(2000);                 // display the above for two seconds
}