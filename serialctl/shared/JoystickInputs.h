#pragma once

#include <stdint.h>
#ifdef __cplusplus
namespace serialctl { namespace internal {
extern "C" {
#endif

typedef struct {
	int8_t stickRX;
	int8_t stickRY;
	int8_t stickLX;
	int8_t stickLY;
	uint8_t btnhi;
	uint8_t btnlo;
} JoystickInputs;

#ifdef __cplusplus
static_assert(sizeof(JoystickInputs) == 6, "Expected JoystickInputs to be 6 bytes");
}; // extern "C"
}} // namespace serialctl namespace internal

namespace serialctl {
# define enumclass(name) enum class name : uint8_t
# define enumdef(name, value) name = value
#else
# define enumclass(name) enum name
# define enumdef(name, value) stick_##name = value
#endif
enumclass(Stick) {
	enumdef(rx, 0),
	enumdef(ry, 1),
	enumdef(lx, 2),
	enumdef(ly, 3),
};
#ifndef __cplusplus
#undef enumdef
#define enumdef(name, value) button_##name = value
#endif
enumclass(Button) {
	enumdef(mainLeft,  0),
	enumdef(mainDown,  1),
	enumdef(mainRight, 2),
	enumdef(mainUp,    3),
	enumdef(triggerUpperLeft,  4),
	enumdef(triggerUpperRight, 5),
	enumdef(triggerLowerLeft,  6),
	enumdef(triggerLowerRight, 7),
	enumdef(select, 8),
	enumdef(start,  9),
	enumdef(leftStickButton,  10),
	enumdef(rightStickButton, 11),
	enumdef(dpadLeft,  12),
	enumdef(dpadDown,  13),
	enumdef(dpadRight, 14),
	enumdef(dpadUp,    15),
};
#ifdef __cplusplus
} // namespace serialctl
#endif
#undef enumdef
#undef enumclass