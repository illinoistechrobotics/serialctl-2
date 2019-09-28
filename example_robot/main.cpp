#include <Arduino.h>
#include <RobotBase.hpp>

class TestRobot: public serialctl::Robot</* Name of class goes here */ TestRobot, /* Number of periodic functions */ 4> {
public:
	void setup() override {
		registerPeriodic(F("Hi"), &TestRobot::test, 100);
	}

	void test() {
		Serial.print(F("Hello!"));
	}
};

TestRobot robot;

void setup() {
	robot.setup();
}

void loop() {
	robot.loop();
}
