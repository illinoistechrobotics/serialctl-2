#pragma once

#include <util/crc16.h>
#include "ArrayReference.hpp"

namespace serialctl {

class StreamingCRC16Calculator {
	uint16_t crc = 0xffff;
public:
	StreamingCRC16Calculator() = default;

	inline uint16_t value() { return crc; }

	inline void reset() { crc = 0xffff; }

	inline void update(uint8_t byte) {
		crc = _crc16_update(crc, byte);
	}
};

uint16_t compute_crc16(ArrayReference<const uint8_t> data);

} // namespace serialctl