#ifndef TUNES_H_
#define TUNES_H_

#include "Arduino.h"

class Tunes
{
  public:
    Tunes(int pin);

    void alive();

    void dead();

    void shoot();

    void shot();

    void play(unsigned int _initialFrequency, int _step, unsigned int _length);

    void heartbeat();

  private:
    const byte TEMPO = 1;

    int pin;
    unsigned long nextAction = 0;
    unsigned int currFrequency = 0;
    unsigned int stopAt = 0;
    int step = 0;
};

#endif /* TUNES_H_ */
