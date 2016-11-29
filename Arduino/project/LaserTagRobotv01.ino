#include "Arduino.h"
#include "Tunes.h"
#include "LaserEmitter.h"
#include "LaserReceiver.h"
#include "RGBLED.h"
#include "Motor.h"
#include "Motors.h"
#include "SpeedSensor.h"
#include <SoftwareSerial.h>
#include "RobotConstants.h"

const byte SPK_PIN = 13;

const byte BT_RX_PIN = 6;
const byte BT_TX_PIN = 5;

const byte MOTOR_LEFT_SENSOR_PIN = 3;
const byte MOTOR_LEFT_FW_PIN = 11;
const byte MOTOR_LEFT_BW_PIN = 10;
const byte MOTOR_RIGHT_SENSOR_PIN = 2;
const byte MOTOR_RIGHT_FW_PIN = 9;
const byte MOTOR_RIGHT_BW_PIN = 8;

const byte RGB_LED_R_PIN = A5;
const byte RGB_LED_G_PIN = A4;
const byte RGB_LED_B_PIN = A3;

const byte LASER_EMITTER_PIN = A1;
const byte LASER_RECEIVER_PIN = A0;

Tunes tunes(SPK_PIN);

SoftwareSerial btSerial(BT_RX_PIN, BT_TX_PIN);

Motor motorLeft = Motor(MOTOR_LEFT_FW_PIN, MOTOR_LEFT_BW_PIN);
Motor motorRight = Motor(MOTOR_RIGHT_FW_PIN, MOTOR_RIGHT_BW_PIN);

SpeedSensor speedSensorLeft = SpeedSensor(MOTOR_LEFT_SENSOR_PIN);
SpeedSensor speedSensorRight = SpeedSensor(MOTOR_RIGHT_SENSOR_PIN);

Motors motors = Motors(&motorLeft, &speedSensorLeft, &motorRight, &speedSensorRight);

LaserEmitter laserEmitter = LaserEmitter(LASER_EMITTER_PIN);
LaserReceiver laserReceiver = LaserReceiver(LASER_RECEIVER_PIN);

RGBLED rgbLED = RGBLED(RGB_LED_R_PIN, RGB_LED_G_PIN, RGB_LED_B_PIN);

String command;

void setup()
{
    Serial.begin(9600);
    Serial.println("Arduino is ready");

    btSerial.begin(9600);
    broadcast("Setup complete!");

    // Tell the world you've come to life
    tunes.alive();
    rgbLED.glow(255, 255, 255, 1000);
}

// Utilities

void broadcast(const char* message) {
	btSerial.print(message);
	Serial.print(message);
}

// Command processing

void readCommand() {
    if (btSerial.available())
    {
    	command = btSerial.readStringUntil(';');
    } else if (Serial.available()) {
    	command = Serial.readStringUntil(';');
    }
}

void processCommand() {
    if (!command || command.length() == 0) {
    	return;
    }

	if (command.charAt(0) == 'p') {
		broadcast("pong");
	} else if (command.charAt(0) == 'l') {
		byte payload = command.substring(1, 4).toInt();
		motors.leftWheel(FORWARD, payload);
	} else if (command.charAt(0) == 'r') {
		byte payload = command.substring(1, 4).toInt();
		motors.rightWheel(FORWARD, payload);
	} else if (command.charAt(0) == 'f') {
		byte payload = command.substring(1, 4).toInt();
		motors.straight(FORWARD, payload);
	} else if (command.charAt(0) == 'b') {
		byte payload = command.substring(1, 4).toInt();
		motors.straight(BACKWARD, payload);
	} else if (command.charAt(0) == 's') {
		motors.stop();
	} else if (command.charAt(0) == 't') {
		laserEmitter.shoot();
		tunes.shoot();
		rgbLED.glow(10, 255, 255, 250);
	} else if (command.charAt(0) == 'd') {
		broadcast("Nothing to debug");
	}

	command = "";
}

// Game logic
unsigned long lastTimeShot = 0;

void gameLogic() {
	// Nothing can happen 1s after being shot... You are immune
	unsigned short value = laserReceiver.readAndClear();
	if ((lastTimeShot + 1000) < millis()) {
		if (value > 10) {
			tunes.shot();
			rgbLED.glow(255, 0, 0, 250);
			lastTimeShot = millis();
		}
	}
}

void loop()
{
	readCommand();
	processCommand();
	gameLogic();
	motors.heartbeat();
	tunes.heartbeat();
	laserEmitter.heartbeat();
	laserReceiver.heartbeat();
	rgbLED.heartbeat();
}
