#include "LaserReceiver.h"

LaserReceiver::LaserReceiver(byte _pin)
{
	pin = _pin;
	pinMode(_pin, INPUT);
	damage = 0;
}

unsigned short LaserReceiver::readAndClear() {
	unsigned short result = damage;
	damage = 0;
	return result;
}

void LaserReceiver::heartbeat() {
	unsigned short value = analogRead(pin);
	value = 255 - map(value, 0, 1023, 0, 255);
	damage = damage > value ? damage : value;
}
