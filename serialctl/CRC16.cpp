#include <Arduino.h>
#include "CRC16.hpp"

uint16_t serialctl::compute_crc16(serialctl::ArrayReference<const uint8_t> data) {
	serialctl::StreamingCRC16Calculator calculator;
	for (auto byte : data) {
		calculator.update(byte);
	}
	return calculator.value();
}