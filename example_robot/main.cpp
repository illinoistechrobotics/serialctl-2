#include "Arduino.h"
#include "../robot/serialctl.h"

void setup() {
	serialctl_init();
}

void loop() {
	serialctl_loop();
}