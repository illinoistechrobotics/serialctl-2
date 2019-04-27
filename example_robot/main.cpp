#include <Arduino.h>
#include <serialctl.h>

void setup() {
	serialctl_init();
}

void loop() {
	serialctl_loop();
}
