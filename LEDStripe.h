#ifndef LEDStripe_h
#define LEDStripe_h

class LEDStripe {
  public:
    LEDStripe(const int redPin, const int greenPin, const int bluePin);

    void powerOn();

    void powerOff();

    void togglePower();

    void setColor(int red, int green, int blue);

    void setRed(int value);

    void setGreen(int value);

    void setBlue(int value);

    boolean getPower();
    
    // TODO Blink, Fade, ...

  private:
    int _redPin;
    int _greenPin;
    int _bluePin;

    int _redValue = 127;
    int _greenValue = 127;
    int _blueValue = 127;
    boolean _power = false;

    void updateValues();

    void setOutputMode();
};
#endif
