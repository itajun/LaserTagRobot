#ifndef LASER_RECEIVER_H_
#define LASER_RECEIVER_H_

#include "Arduino.h"

class LaserReceiver
{
  public:
	LaserReceiver(byte pin);

    unsigned short readAndClear();

    void heartbeat();
  private:
    byte pin;
    byte damage;
};

#endif /* LASER_RECEIVER */
