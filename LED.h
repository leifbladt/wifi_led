#ifndef LED_h
#define LED_h

class LED {
  public:
    LED(const int pin);

    void powerOn();

    void powerOff();

    // TODO Blink, Fade, ...

  private:
    int _pin;
};
#endif
