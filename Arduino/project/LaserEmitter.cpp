#include "LaserEmitter.h"

LaserEmitter::LaserEmitter(byte _pin)
{
	pin = _pin;
	pinMode(_pin, OUTPUT);
	shootingStarted = 0;
}

void LaserEmitter::shoot() {
	if (shootingStarted > 0) {
		return;
	}

	digitalWrite(pin, HIGH);
	shootingStarted = millis();
}

void LaserEmitter::heartbeat() {
	if (shootingStarted == 0) {
		return;
	} else if ((shootingStarted + DURATION) <= millis()) {
		digitalWrite(pin, LOW);
		shootingStarted = 0;
	}
}
