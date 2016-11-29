#ifndef MOTORS_H_
#define MOTORS_H_

#include "Arduino.h"
#include "Motor.h"
#include "SpeedSensor.h"

class Motors {
public:
	Motors(Motor *_motorLeft, SpeedSensor *_speedSensorLeft, Motor *_motorRight,
			SpeedSensor *_speedSensorRight);

	void stop();

	void leftWheel(byte direction, byte speed);

	void rightWheel(byte direction, byte speed);

	void straight(byte direction, byte speed);

	void heartbeat();
private:
	const unsigned short CALIBRATION_CYCLE = 200;

	unsigned long nextCycle = 0;
	byte lockDirection = 255;
	boolean lockWheels = false;
	byte maxSpeed = 255;
	byte bestDeviation = 255;
	byte bestRightSpeed = maxSpeed;
	byte bestLeftSpeed = maxSpeed;
	short rightSpeed = 0;
	short leftSpeed = 0;

	Motor *motorLeft;
	SpeedSensor *speedSensorLeft;

	Motor *motorRight;
	SpeedSensor *speedSensorRight;
};

#endif /* MOTORS_H_ */
