#pragma once
#include "ArrayReference.hpp"

namespace serialctl {

template <typename T, size_t Size>
struct Array {
	T _data[Size];

	T* data() { return _data; }
	constexpr const T* data() const { return _data; }

	constexpr size_t size() const { return Size; }

	static Array repeating(const T& element) {
		Array arr;
		for (T& item : arr) {
			item = element;
		}
		return arr;
	}

	T* begin() { return _data; }
	constexpr const T* begin() const { return _data; }

	T* end() { return _data + Size; }
	constexpr const T* end() const { return _data + Size; }

	T& operator[](size_t index) { return _data[index]; }
	constexpr const T& operator[](size_t index) const { return _data[index]; }

	operator ArrayReference<T>() { return {_data, Size}; }
	constexpr operator ArrayReference<const T>() const { return {_data, Size}; }
};

} /* namespace serialctl */
