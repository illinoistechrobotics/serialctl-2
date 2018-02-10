#include "Arduino.h"

void serialctl_init() {
	pinMode(13, OUTPUT);
}

void serialctl_loop() {
	digitalWrite(13, HIGH);
	delay(200);
	digitalWrite(13, LOW);
	delay(200);
}
