#include "Motors.h"
#include "Motor.h"
#include "SpeedSensor.h"
#include "RobotConstants.h"

Motors::Motors(Motor *_motorLeft, SpeedSensor *_speedSensorLeft,
		Motor *_motorRight, SpeedSensor *_speedSensorRight) {
	motorLeft = _motorLeft;
	speedSensorLeft = _speedSensorLeft;
	motorRight = _motorRight;
	speedSensorRight = _speedSensorRight;
}

void Motors::stop() {
	lockWheels = false;
	motorLeft->stop();
	motorRight->stop();
}

void Motors::leftWheel(byte direction, byte speed) {
	lockWheels = false;
	motorLeft->run(direction, speed);
}

void Motors::rightWheel(byte direction, byte speed) {
	lockWheels = false;
	motorRight->run(direction, speed);
}

void Motors::straight(byte direction, byte speed) {
	lockWheels = true;
	if (lockDirection != direction || speed != maxSpeed) {
		lockDirection = direction;
		bestDeviation = 255;
		maxSpeed = bestLeftSpeed = bestRightSpeed = leftSpeed = rightSpeed =
				speed;
	}
	speedSensorLeft->readAndReset();
	speedSensorRight->readAndReset();
	motorLeft->run(direction, speed);
	motorRight->run(direction, speed);
}

void Motors::heartbeat() {
	if (nextCycle > millis() || !lockWheels) {
		return;
	}

	unsigned long left = speedSensorLeft->readAndReset();
	unsigned long right = speedSensorRight->readAndReset();

	unsigned short total = left + right;
	short deviation = left - right;
	unsigned short absDeviation = abs(deviation);

	if (absDeviation < bestDeviation) {
		bestLeftSpeed = leftSpeed;
		bestRightSpeed = rightSpeed;
		bestDeviation = absDeviation;
	}

	if (lockDirection == BACKWARD) {
		deviation *= -1;
	}

	if (absDeviation > 0) {
		byte cycleBias = maxSpeed * absDeviation / total;

		// < 0 right too fast
		if (deviation < 0 && right >= left) {
			rightSpeed -= cycleBias;
		} else if (deviation > 0 && left >= right) {
			leftSpeed -= cycleBias;
		}

		if (leftSpeed < rightSpeed) {
			leftSpeed += maxSpeed - rightSpeed;
			rightSpeed = maxSpeed;
		} else if (rightSpeed < leftSpeed) {
			rightSpeed += maxSpeed - leftSpeed;
			leftSpeed = maxSpeed;
		}

	}

	motorLeft->run(lockDirection, leftSpeed);
	motorRight->run(lockDirection, rightSpeed);

	nextCycle = millis() + CALIBRATION_CYCLE;
}
