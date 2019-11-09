#pragma once

#include <stdint.h>
#ifdef __cplusplus
# define enumclass(name) enum class name : uint8_t
# define enumdef(name, value) name = value
namespace serialctl {
extern "C" {
#else
# define enumclass(name) enum name
# define enumdef(name, value) serialctl_opcode_##name = value
#endif
enumclass(SerialctlOpcode) {
	enumdef(identify, 0x00),
	enumdef(identifyResponse, 0x01),
	enumdef(joystickData, 0x02),
	enumdef(robotStatus, 0x03),
	enumdef(logMessage, 0x04),
	enumdef(errorMessage, 0x05)
};

#ifdef __cplusplus
} // extern C
} // namespace serialctl
#endif
#undef enumdef
#undef enumclass