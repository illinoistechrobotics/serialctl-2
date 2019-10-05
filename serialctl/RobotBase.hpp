#pragma once

#include "Array.hpp"
#include "ArrayReference.hpp"
#include "JoystickInputs.hpp"

namespace serialctl {
namespace internal {
/**
 * Get a JoystickInputs that is unlikely to cause the robot to do anything bad, used for estop
 */
constexpr serialctl::JoystickInputs safeJoystickInput() {
	return {0, 0, 0, 0, 0};
}
} // namespace internal

template <typename Subclass, int MaxPeriodicMethods>
class Robot {
private:
	struct PeriodicMethod {
		unsigned long mostRecentRun;
		void (Subclass::*method)();
		const __FlashStringHelper *name;
		uint16_t msDelay;
		uint16_t timeTakenByMostRecentRun;
	};

	/**
	 * Information about the periodic methods registered to run
	 * (Modify this array using registerPeriodic and deletePeriodic)
	 */
	Array<PeriodicMethod, MaxPeriodicMethods> periodicMethods;

	uint16_t timeTakenByLastOnReceivePacket = 0;

	/**
	 * The last joystick input, not affected by estop
	 */
	JoystickInputs lastJoystickInput = internal::safeJoystickInput();

	bool estop = true;

	Subclass& impl() { return *static_cast<Subclass *>(this); }

	const Stream& SerComm() { return impl().SerComm(); }
public:
	Robot(): periodicMethods() {};

	/**
	 * The main setup function, should be called in the arduino setup function
	 * Overridable, override to do things like register periodic methods and other robot subclass setup.
	 */
	virtual void setup() = 0;

	/**
	 * Run once each time the robot gets a new joystick input from the control software.  The given joystick
	 */
	virtual void onReceivePacket(const JoystickInputs& joystick) = 0;

	/**
	 * The main loop function, should be called in the arduino loop function
	 * Not overridable, register a periodic method instead of overriding.
	 */
	void loop() {
		SerComm().read();

		unsigned long start = millis();
		for (auto& periodic : periodicMethods) {
			if (periodic.method == nullptr) { continue; }
			if (start - periodic.mostRecentRun < periodic.msDelay) { return; }
			periodic.mostRecentRun = start;
			unsigned long timer = millis();
			(impl().*periodic.method)();
			periodic.timeTakenByMostRecentRun = millis() - timer;
		}
	}

	/**
	 * Registers a method to be run periodically
	 * @param name The name of the method (for debugging purposes)
	 * @param method The method itself
	 * @param msDelay How long to wait between calls to the method
	 * @return Whether or not the registration was successful.  This can fail if you try to register more than @p MaxPeriodicFunctions methods
	 */
	bool registerPeriodic(const __FlashStringHelper *name, void (Subclass::*method)(), uint16_t msDelay) {
		for (auto& item : periodicMethods) {
			if (item.method == nullptr) {
				item = { .mostRecentRun = 0, method, name, .msDelay = msDelay, .timeTakenByMostRecentRun = 0 };
				return &item - &periodicMethods[0];
			}
		}
		return -1;
	}

	/**
	 * Deregisters the given method so it is no longer called periodically
	 * @param method the method to deregister
	 */
	void deregisterPeriodic(void (Subclass::*method)()) {
		for (auto& item : periodicMethods) {
			if (item.method == method) {
				item.method = nullptr;
			}
		}
	}

	/**
	 * Gets the actual joystick input, not zeroed by estop.
	 *
	 * Use for checking buttons to turn things on and off, since you don't want those to get disabled by estop
	 */
	const JoystickInputs& getActualJoystick() {
		return lastJoystickInput;
	}
};

} // namespace serialctl