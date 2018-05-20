/* Simple button test for the WaveShare 4x4 AD Keypad, using 5V for GCC
*/

#include "ardprintf.h" // for ardprintf() function; see https://arduino.stackexchange.com/a/201/25757

int buttonNum; // the current reading from the input pin
int lastButtonNum = -1; // the previous reading from the input pin

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() {
  Serial.begin(9600);
}

/** Based on the voltage (5V in), determine which key was pressed 
 *  @see https://www.robotics.org.za/W4431?search=keypad&description=true
*/
int getKey(float voltage) {
  int out = -1;
  if (voltage < 0.1) out = 0;
  else if (voltage < 0.22) out = 1;
  else if (voltage < 0.35) out = 2;
  else if (voltage < 0.6) out = 3;
  else if (voltage < 0.75) out = 4;
  else if (voltage < 1.00) out = 5;
  else if (voltage < 1.20) out = 6;
  else if (voltage < 1.45) out = 7;
  else if (voltage < 1.70) out = 8;
  else if (voltage < 2.0) out = 9;
  else if (voltage < 2.35) out = 10;
  else if (voltage < 2.65) out = 11;
  else if (voltage < 3.05) out = 12;
  else if (voltage < 3.50) out = 13;
  else if (voltage < 3.95) out = 14;
  else if (voltage < 4.50) out = 15;

  return out;
}

void loop() {
  // read the raw data coming in on analog pin 0:
  int rawValue = analogRead(A0);
  // Convert the raw data value (0 - 1023) to voltage (0.0V - 5.0V):
  float voltage = rawValue * (5.0 / 1024.0);
  int keyNum = getKey(voltage); // get the number of the key, based on voltage
  // filter/debounce: Only one output per key press
  // If the switch changed, due to noise or pressing:
  if (keyNum != lastButtonNum) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer than the debounce
    // delay, so take it as the actual current state:
    // if the button state has changed:
    if (keyNum != buttonNum) {
      buttonNum = keyNum;
      // only output the button number if the new button number is for a valid button
      if (buttonNum != -1) {
        ardprintf("Key: %d", keyNum); // write the button value to the serial monitor, if not -1:
        // ardprintf("Voltage: %f\tKey: %d", voltage, keyNum);
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonNum:
  lastButtonNum = keyNum;
}
