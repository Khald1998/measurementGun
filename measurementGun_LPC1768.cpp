#include "mbed.h"

#include "TextLCD.h"

#include "hcsr04.h"


#define TRIG_PIN p23
#define ECHO_PIN p21

#define buzzerPin p22

#define BUTTON_PIN p20

#define RS p5
#define E p6
#define D4 p7
#define D5 p8
#define D6 p9
#define D7 p10

int lastButtonState; // the previous state of button
int currentButtonState; // the current state of button
bool offOrOn = false; // the current state of device

TextLCD lcd(RS, E, D4, D5, D6, D7); // rs, e, d4-d7
HCSR04 sensor(TRIG_PIN, ECHO_PIN);
PwmOut buzz(buzzerPin);
DigitalIn BUTTON(BUTTON_PIN);
void state() {
  int distance = sensor.distance();
  distance = (distance > 1200) ? 0 : distance;
  int n = (distance < 9) ? 3 : ((distance < 99) ? 2 : ((distance < 999) ? 1 : 0));
  float m = static_cast < float > (distance);
  buzz.period(1.0 / m * 4);
  buzz = 0.5;

  lcd.locate(0, 0);
  lcd.printf("The readings are");
  lcd.locate(0, 1);
  lcd.printf("0000");
  lcd.locate(n, 1);
  lcd.printf("%d  \n", distance);
  lcd.locate(4, 1);
  lcd.printf("cm");
  ThisThread::sleep_for(20ms);

}
void offState() {
  buzz.write(0.0);  
  lcd.locate(0, 0);
  lcd.printf("The device is    ");
  lcd.locate(0, 1);
  lcd.printf("off              ");
}

int main() {
  currentButtonState = BUTTON.read();

  while (true) {
    lastButtonState = currentButtonState; // save the last state
    currentButtonState = BUTTON.read(); // read new state
    if (lastButtonState == 1 && currentButtonState == 0) {
      offOrOn = !offOrOn;
    }
    if (offOrOn == true) {
      state();
    } else {

      offState();

    }

  }
}