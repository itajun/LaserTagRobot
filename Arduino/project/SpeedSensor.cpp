#include "SpeedSensor.h"
#include "RobotConstants.h"

/**
 * Must be a static function, so we keep track of all interrupts in the same file
 */
unsigned long pin2Value = 0;
unsigned long pin3Value = 0;


void interruptPin2() {
	pin2Value++;
}

void interruptPin3() {
	pin3Value++;
}


SpeedSensor::SpeedSensor(byte _pin)
{
	pin = _pin;
	pinMode(_pin, INPUT);

	if (_pin == 2) {
		attachInterrupt(
				digitalPinToInterrupt(_pin),
				interruptPin2, CHANGE);
	} else {
		attachInterrupt(
				digitalPinToInterrupt(_pin),
				interruptPin3, CHANGE);
	}
}

unsigned long SpeedSensor::readAndReset() {
	unsigned long response = read();
	if (pin == 2) {
		pin2Value = 0;
	} else {
		pin3Value = 0;
	}
	return response;
}

unsigned long SpeedSensor::read() {
	if (pin == 2) {
		return pin2Value;
	} else {
		return pin3Value;
	}
}
