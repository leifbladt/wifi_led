#include "Arduino.h"
#include "LEDStripe.h"

// TODO Persist color values

LEDStripe::LEDStripe(const int redPin, const int greenPin, const int bluePin) {
  _redPin = redPin;
  _greenPin = greenPin;
  _bluePin = bluePin;

  setOutputMode();
}

void LEDStripe::powerOn() {
  Serial.println("switch on");

  _redValue = 127;
  _greenValue = 127;
  _blueValue = 127;
  updateValues();
}

void LEDStripe::powerOff() {
  Serial.println("switch off");

  _redValue = 0;
  _greenValue = 0;
  _blueValue = 0;
  updateValues();
}

void LEDStripe::togglePower() {
  Serial.println("toggle power");

  powerOn();
  // TODO Implement me!
}

void LEDStripe::setColor(int red, int green, int blue) {
  _redValue = red;
  _greenValue = green;
  _blueValue = blue;
  updateValues();
}

void LEDStripe::setRed(int value) {
  Serial.print("red value: ");
  Serial.println(value);

  _redValue = value;
  updateValues();
}

void LEDStripe::setGreen(int value) {
  Serial.print("green value: ");
  Serial.println(value);

  _greenValue = value;
  updateValues();
}

void LEDStripe::setBlue(int value) {
  Serial.print("blue value: ");
  Serial.println(value);

  _blueValue = value;
  updateValues();
}

void LEDStripe::updateValues() {
  analogWrite(_redPin, _redValue);
  analogWrite(_greenPin, _greenValue);
  analogWrite(_bluePin, _blueValue);
}

void LEDStripe::setOutputMode() {
  pinMode(_redPin, OUTPUT);
  pinMode(_greenPin, OUTPUT);
  pinMode(_bluePin, OUTPUT);
}
