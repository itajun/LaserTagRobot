#include "RGBLED.h"

RGBLED::RGBLED(byte r, byte g, byte b)
{
	pinR = r;
	pinG = g;
	pinB = b;
	pinMode(pinR, OUTPUT);
	pinMode(pinG, OUTPUT);
	pinMode(pinB, OUTPUT);
	glowingStarted = 0;
	duration = 0;
}

void RGBLED::glow(unsigned short r, unsigned short g, unsigned short b, unsigned short _duration) {
	if (glowingStarted > 0) {
		return;
	}

	analogWrite(pinR, r);
	analogWrite(pinG, g);
	analogWrite(pinB, b);
	duration = _duration;
	glowingStarted = millis();
}

void RGBLED::heartbeat() {
	if (glowingStarted == 0) {
		return;
	} else if ((glowingStarted + duration) <= millis()) {
		digitalWrite(pinR, LOW);
		digitalWrite(pinG, LOW);
		digitalWrite(pinB, LOW);
		duration = glowingStarted = 0;
	}
}
