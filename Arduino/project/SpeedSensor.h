#ifndef SPEED_SENSOR_H_
#define SPEED_SENSOR_H_

#include "Arduino.h"

class SpeedSensor
{
  public:
	SpeedSensor(byte pin);

    unsigned long read();

    unsigned long readAndReset();
  private:
    byte pin;
};

#endif /* SPEED_SENSOR_H_ */
