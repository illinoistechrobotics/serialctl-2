#include <Arduino.h>
#include <RobotBase.hpp>

class TestRobot: public serialctl::Robot</* Name of class goes here */ TestRobot, /* Number of periodic functions */ 4> {
public:
	// This needs to be defined for the RobotBase to work, make it return the serial device you want to communicate over
	static const Stream& SerComm() { return Serial; };

	void setup() override {
		registerPeriodic(F("Hi"), &TestRobot::test, 100);
	}

	void onReceivePacket(const serialctl::JoystickInputs& joystick) override {

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
