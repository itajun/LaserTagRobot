#ifndef RGB_LED_H
#define RGB_LED_H

#include "Arduino.h"

class RGBLED
{
  public:
	RGBLED(byte pinR, byte pinG, byte pinB);

    void glow(unsigned short r, unsigned short g, unsigned short b, unsigned short duration);

    void heartbeat();
  private:
    byte pinR;
    byte pinG;
    byte pinB;
    unsigned short duration;
    unsigned long glowingStarted;
};

#endif /* RGB_LED_H */
