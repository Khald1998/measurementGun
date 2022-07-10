
/*
  The circuit:
   LCD RS pin to digital pin 7
   LCD Enable pin to digital pin 8
   LCD D4 pin to digital pin 9
   LCD D5 pin to digital pin 10
   LCD D6 pin to digital pin 11
   LCD D7 pin to digital pin 12
   LCD R/W pin to ground
   LCD VSS pin to ground
   LCD VCC pin to 5V
   10K resistor:
   ends to +5V and ground
   wiper to LCD VO pin (pin 3)
*/

// include the library code:
#include <LiquidCrystal.h>
#include "SR04.h"

#define TRIG_PIN 11
#define ECHO_PIN 12
#define buzzer 8
#define BUTTON_PIN 13
#define RS 7
#define E 6
#define D4 5
#define D5 4
#define D6 3
#define D7 2


SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);

int distance, distanceBefore;
int lastButtonState;    // the previous state of button
int currentButtonState; // the current state of button
int offOrOn = LOW;     // the current state of device
bool first = false;

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(RS, E , D4, D5, D6, D7);

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  currentButtonState = digitalRead(BUTTON_PIN);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  offState();
}

void loop() {
  lastButtonState    = currentButtonState;      // save the last state
  currentButtonState = digitalRead(BUTTON_PIN); // read new state

  if (lastButtonState == HIGH && currentButtonState == LOW) {

    offOrOn = !offOrOn;
  }
  if (offOrOn == HIGH)
  {
    first = false;
    state();
  }
  else
  {
    if (first == false)
    {
      lcd.clear();
      noTone(buzzer);
      first = true;
      offState();
    }


  }

}
void state()
{

  lcd.setCursor(0, 0);
  lcd.print("The reading is  ");

  distance = sr04.Distance();
  distance = (distance > 1180) ? 0 : distance;
  tone(buzzer, distance * 4);


  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):

  lcd.setCursor(4, 1);
  lcd.write("cm");

  if (distanceBefore != distance)
  {
    lcd.setCursor(0, 1);
    lcd.print("0000");

  }
  if (distance < 9)
  {
    lcd.setCursor(3, 1);
    lcd.print(distance);
    delay(20);

  }
  else if (distance < 99)
  {
    lcd.setCursor(2, 1);
    lcd.print(distance);
    delay(20);

  }
  else if (distance < 999)
  {
    lcd.setCursor(1, 1);
    lcd.print(distance);
    delay(20);

  }
  else
  {
    lcd.setCursor(0, 1);
    lcd.print(distance);
    delay(20);

  }
  distanceBefore = distance;



}
void offState()
{
  lcd.print("The device is    ");
  lcd.setCursor(0, 1);
  lcd.print("off              ");
}
