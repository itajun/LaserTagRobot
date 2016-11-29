#include "Tunes.h"
#include "Arduino.h"

Tunes::Tunes(int _pin)
{
  pinMode(_pin, OUTPUT);
  pin = _pin;
}

void Tunes::alive() {
	play(500, 1, 2000);
}

void Tunes::dead() {
	play(2500, -1, 500);
}

void Tunes::shoot() {
	play(100, 5, 100);
}

void Tunes::shot() {
	play(50, 0, 100);
}

void Tunes::play(unsigned int _initialFrequency, int _step, unsigned int _length) {
  if (stopAt > millis()) {
	  return;
  }

  currFrequency = _initialFrequency;
  step = _step;
  stopAt = millis() + _length;
  tone(pin, currFrequency);
}

void Tunes::heartbeat() {
	if (stopAt == 0 || nextAction > millis()) {
		return;
	}

	if (millis() > stopAt) {
		noTone(pin);
		stopAt = 0;
		currFrequency = 0;
		return;
	}

	currFrequency += step;
	tone(pin, currFrequency);
	nextAction = millis() + TEMPO;
}
