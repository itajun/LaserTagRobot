#ifndef MOTOR_H_
#define MOTOR_H_

#include "Arduino.h"

class Motor
{
  public:
	Motor(byte fwPin, byte bwPin);

    void stop();

    void run(byte direction, byte speed);

  private:
    byte fwPin;
    byte bwPin;
};

#endif /* MOTOR_H_ */
