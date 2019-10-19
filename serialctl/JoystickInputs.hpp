#pragma once

#include "shared/JoystickInputs.h"
#include "Array.hpp"
#include "Utility.hpp"

namespace serialctl {
struct JoystickInputs {
private:
	Array<int8_t, 4> sticks;
	uint16_t buttons;
public:
	explicit inline JoystickInputs(internal::JoystickInputs inputs):
		sticks({inputs.stickRX, inputs.stickRY, inputs.stickLX, inputs.stickLY}),
		buttons(concat_uint8(inputs.btnhi, inputs.btnlo))
	{}

	constexpr JoystickInputs(int8_t rx, int8_t ry, int8_t lx, int8_t ly, uint16_t btn):
		sticks({rx, ry, lx, ly}),
		buttons(btn)
	{}

	inline int8_t getStick(Stick stick) {
		return sticks[(uint8_t)stick];
	}

	inline bool getButton(Button button) {
		return ((buttons >> (uint8_t)button) & 1) != 0;
	}
};
} // namespace serialctl