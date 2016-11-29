#ifndef LASER_EMITTER_H_
#define LASER_EMITTER_H_

#include "Arduino.h"

class LaserEmitter
{
  public:
	LaserEmitter(byte pin);

    void shoot();

    void heartbeat();
  private:
    const unsigned short DURATION = 1000;

    byte pin;
    unsigned long shootingStarted;
};

#endif /* LASER_EMITTER */
