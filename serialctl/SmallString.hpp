#pragma once

#include "SmallVector.hpp"

namespace serialctl {

template<size_t Capacity>
class SmallString: public SmallVector<char, Capacity> {
public:
	using SmallVector<char, Capacity>::SmallVector;
	explicit SmallString(const SmallVector<char, Capacity>& vector): SmallVector<char, Capacity>(vector) {}
	operator StringReference() { return StringReference{ArrayReference<char>(*this)}; }
	operator ConstStringReference() const { return ConstStringReference{ArrayReference<const char>{*this}}; }
};

} /* namespace serialctl */
