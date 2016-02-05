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

  _power = true;
  updateValues();
}

void LEDStripe::powerOff() {
  Serial.println("switch off");

  _power = false;
  updateValues();
}

void LEDStripe::togglePower() {
  Serial.println("toggle power");

  _power = !_power;
  updateValues();
}

void LEDStripe::setColor(int red, int green, int blue) {
  Serial.print("set color (");
  Serial.print(red);
  Serial.print("/");
  Serial.print(green);
  Serial.print("/");
  Serial.print(blue);
  Serial.println(")");

  _power = true;
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
  analogWrite(_redPin, _power ? _redValue : 0);
  analogWrite(_greenPin, _power ? _greenValue : 0);
  analogWrite(_bluePin, _power ? _blueValue : 0);
}

void LEDStripe::setOutputMode() {
  pinMode(_redPin, OUTPUT);
  pinMode(_greenPin, OUTPUT);
  pinMode(_bluePin, OUTPUT);
}
