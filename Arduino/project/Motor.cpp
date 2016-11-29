#include "Motor.h"
#include "RobotConstants.h"

Motor::Motor(byte _fwPin, byte _bwPin)
{
	fwPin = _fwPin;
	bwPin = _bwPin;

	pinMode(fwPin, OUTPUT);
	pinMode(bwPin, OUTPUT);
}

void Motor::stop() {
	digitalWrite(fwPin, LOW);
	digitalWrite(bwPin, LOW);
}

void Motor::run(byte direction, byte speed) {
	if (direction == FORWARD) {
		digitalWrite(bwPin, LOW);
		analogWrite(fwPin, speed);
	} else if (direction == BACKWARD) {
		digitalWrite(fwPin, LOW);
		analogWrite(bwPin, speed);
	}
}
