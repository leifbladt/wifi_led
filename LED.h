#ifndef LED_h
#define LED_h

class LED {
  public:
    LED(const int pin);

    void powerOn();

    void powerOff();

    void blink(unsigned int frequency);

    void loop();

    // TODO Fade, ...

  private:
    int _pin;
    enum LEDMode { SHINE, BLINK };
    LEDMode _mode = SHINE; 
    long _lastStart;
    unsigned int _duration;
};
#endif
