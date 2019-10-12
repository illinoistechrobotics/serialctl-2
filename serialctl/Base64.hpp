#pragma once

#include "SmallVector.hpp"

namespace serialctl {

class StreamingBase64Decoder {
	Array<uint8_t, 4> buffer;
	uint8_t size = 0;

	static inline void a3_to_a4(uint8_t *a4, const uint8_t *a3) {
		a4[0] = (a3[0] & 0xfc) >> 2;
		a4[1] = ((a3[0] & 0x03) << 4) | ((a3[1] & 0xf0) >> 4);
		a4[2] = ((a3[1] & 0x0f) << 2) | ((a3[2] & 0xc0) >> 6);
		a4[3] = (a3[2] & 0x3f);
	}

	static inline void a4_to_a3(uint8_t *a3, const uint8_t *a4) {
		a3[0] = (a4[0] << 2) | ((a4[1] & 0x30) >> 4);
		a3[1] = ((a4[1] & 0xf) << 4) | ((a4[2] & 0x3c) >> 2);
		a3[2] = ((a4[2] & 0x3) << 6) | a4[3];
	}

	static inline uint8_t b64_lookup(uint8_t c) {
		if(c >='A' && c <='Z') return c - 'A';
		if(c >='a' && c <='z') return c - 'a' + 26;
		if(c >='0' && c <='9') return c - '0' + 52;
		if(c == '+') return 62;
		if(c == '/') return 63;
		return 0;
	}


	static inline void appendByteToOutput(uint8_t byte, ArrayReference<uint8_t> output, uint8_t *outputLength) {
		if (*outputLength < output.size()) {
			output[*outputLength] = byte;
			*outputLength++;
		}
	}
public:
	StreamingBase64Decoder() = default;

	inline void addByte(uint8_t byte, ArrayReference<uint8_t> output, uint8_t *outputLength) {
		buffer[size++] = byte;
		if (size == buffer.size()) {
			size = 0;
			Array<uint8_t, 4> decoded = buffer;
			for (auto& byte : decoded) {
				byte = b64_lookup(byte);
			}
			Array<uint8_t, 3> tmp;
			a4_to_a3(tmp.data(), decoded.data());
			for (auto byte : tmp) {
				appendByteToOutput(byte, output, outputLength);
			}
		}
	}

	inline void finalize(ArrayReference<uint8_t> output, uint8_t *outputLength) {
		while (size != 0) {
			addByte('=', output, outputLength);
		}
		for (auto byte : buffer) {
			if (byte == '=') {
				*outputLength--;
			}
		}
	}
};

} // namespace serialctl