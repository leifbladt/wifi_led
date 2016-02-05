#include "Arduino.h"
#include "LED.h"

LED::LED(const int pin) {
  _pin = pin;
  pinMode(_pin, OUTPUT);
}

void LED::powerOn() {
  Serial.println("power on");
  
  _mode = SHINE;
  digitalWrite(_pin, HIGH);
}

void LED::powerOff() {
  Serial.println("power off");

  _mode = SHINE;
  digitalWrite(_pin, LOW);
}

void LED::blink(unsigned int frequency) {
  Serial.printf("blink [%d]\n", frequency);

  _mode = BLINK;
  _lastStart = millis();
  _duration = 1000.0 / frequency / 2.0;
  digitalWrite(_pin, HIGH);
}

void LED::loop() {


  if (_mode == BLINK) {
    if (millis() - _lastStart < _duration) {
      digitalWrite(_pin, HIGH);
    } else if (millis() - _lastStart < 2 * _duration) {
      digitalWrite(_pin, LOW);
    } else {
      _lastStart = millis();
    }
  }
}

