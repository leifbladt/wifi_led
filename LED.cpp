#include "Arduino.h"
#include "LED.h"

LED::LED(const int pin) {
  _pin = pin;
  pinMode(_pin, OUTPUT);
}

void LED::powerOn() {
  digitalWrite(_pin, HIGH);
}

void LED::powerOff() {
  digitalWrite(_pin, LOW);
}


