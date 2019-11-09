#pragma once

#include "SmallVector.hpp"
#include "Base64.hpp"
#include "CRC16.hpp"

namespace serialctl {

namespace internal {

constexpr uint8_t PACKET_START = 255;
constexpr uint8_t PACKET_END   = 254;

} // namespace internal

// A class for receiving packets
// Call its receive method with new bytes and a callback function that takes an ArrayReference<const uint8_t> and returns nothing
// Each time a full packet is received, the callback function will be called with a reference to it.  The reference is valid until the next time receive is called.
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
			callback(recvBuffer.asRef());
		}
		else {
			decoder.addByte(byte, recvBuffer.asRef(), recvBuffer.sizePtr());
		}
	}
};

} // namespace serialctl