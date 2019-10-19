#pragma once

#include "SmallVector.hpp"
#include "Base64.hpp"
#include "CRC16.hpp"

namespace serialctl {

namespace internal {

constexpr uint8_t PACKET_START = 255;
constexpr uint8_t PACKET_END   = 254;

} // namespace internal

template<size_t BufferSize>
class PacketReceiver {
	SmallVector<uint8_t, BufferSize> recvBuffer;
	StreamingBase64Decoder decoder;
public:
	template <typename Callback>
	void receive(uint8_t byte, Callback callback) {
		if (byte == internal::PACKET_START) {
			decoder.reset();
			recvBuffer.clear();
		}
		else if (byte == internal::PACKET_END) {
			decoder.finalize(recvBuffer.asRef(), recvBuffer.sizePtr());
			if (recvBuffer.size() < 2) { return; }
			uint16_t crc = compute_crc16(recvBuffer.asRef().dropLast(2));
			uint16_t givenCRC = concat_uint8(recvBuffer[recvBuffer.size() - 2], recvBuffer[recvBuffer.size() - 1]);
			if (crc != givenCRC) { return; }
			callback(recvBuffer.asRef().dropLast(2));
		}
		else {
			decoder.addByte(byte, recvBuffer.asRef(), recvBuffer.sizePtr());
		}
	}
};

} // namespace serialctl