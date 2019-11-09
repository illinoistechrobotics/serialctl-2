#pragma once

#include "Array.hpp"
#include "ArrayReference.hpp"
#include "JoystickInputs.hpp"
#include "Base64.hpp"
#include "PacketReceiver.hpp"
#include "shared/Messages.h"

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
	unsigned long mostRecentOnReceivePacket;

	PacketReceiver<63> packetReceiver;

	/**
	 * The last joystick input, not affected by estop
	 */
	JoystickInputs lastJoystickInput = internal::safeJoystickInput();

	bool estop = true;

	// Handles an incoming packet
	// The packet should have already had its CRC16 verified and the CRC should not be included in the data passed to this method
	void handlePacket(ArrayReference<const uint8_t> packet) {
		auto rest = packet.dropFirst(1);
		switch (SerialctlOpcode(packet[0])) {
			case SerialctlOpcode::identify:
				// TODO: Implement this
				break;
			case SerialctlOpcode::identifyResponse:
				// This is sent by the robot, we should never receive one
				break;
			case SerialctlOpcode::joystickData:
				if (rest.size() == sizeof(internal::JoystickInputs)) {
					lastJoystickInput = JoystickInputs(*reinterpret_cast<const internal::JoystickInputs *>(rest.begin()));
					callOnReceivePacket();
				}
			case SerialctlOpcode::robotStatus:
				// This is sent by the robot, we should never receive one
				break;
			case SerialctlOpcode::logMessage:
				// This is sent by the robot, we should never receive one
				break;
			case SerialctlOpcode::errorMessage:
				// This is sent by the robot, we should never receive one
				break;
			default:
				// Currently ignores unknown messages
				break;
		}
	}

	void callOnReceivePacket() {
		uint16_t start = millis();
		if (estop) {
			JoystickInputs safe = internal::safeJoystickInput();
			onReceivePacket(safe);
		}
		else {
			onReceivePacket(lastJoystickInput);
		}
		uint16_t stop = millis();
		timeTakenByLastOnReceivePacket = stop - start;
	}

	Subclass& impl() { return *static_cast<Subclass *>(this); }

	const Stream& SerComm() { return impl().SerComm(); }
public:
	Robot(): periodicMethods() {};

	/**
	 * The main setup function, should be called in the arduino setup function
	 * Overridable, override to do things like register periodic methods and other robot subclass setup.
	 */
	virtual void setup() {
		unsigned long start = millis();
		JoystickInputs inputs = internal::safeJoystickInput();
		onReceivePacket(inputs);
		mostRecentOnReceivePacket = millis();
		timeTakenByLastOnReceivePacket = mostRecentOnReceivePacket - start;
	}

	/**
	 * Run once each time the robot gets a new joystick input from the control software.
	 * The given joystick will be one with safe inputs (all buttons off and sticks centered) if the robot is in estop
	 * If you're controlling something that you want to work during estop, use getActualJoystick
	 */
	virtual void onReceivePacket(const JoystickInputs& joystick) = 0;

	/**
	 * The main loop function, should be called in the arduino loop function
	 * Not overridable, register a periodic method instead of overriding.
	 */
	void loop() {
		while (SerComm().available()) {
			bool shouldStop = false;
			packetReceiver.receive(SerComm().read(), [this, &shouldStop](ArrayReference<const uint8_t> packet) {
				shouldStop = true;
				if (packet.size() < 2) {
					// Packet not big enough for CRC
					return;
				}
				uint16_t crc = compute_crc16(packet.dropLast(2));
				uint16_t givenCRC = concat_uint8(packet[packet.size() - 2], packet[packet.size() - 1]);
				if (crc != givenCRC) {
					// TODO: Report error
					return;
				}
				handlePacket(packet.dropLast(2));
			});
			if (shouldStop) { break; }
		}

		unsigned long start = millis();
		for (auto& periodic : periodicMethods) {
			if (periodic.method == nullptr) { continue; }
			if (start - periodic.mostRecentRun < periodic.msDelay) { return; }
			periodic.mostRecentRun = start;
			unsigned long timer = millis();
			(impl().*periodic.method)();
			periodic.timeTakenByMostRecentRun = millis() - timer;
			start = timer;
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