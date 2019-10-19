#pragma once

#ifndef Arduino_h
#include <string>
#endif
#include "std/type_traits"

namespace serialctl {
#pragma mark -

/// A reference to an array with a known size
/// Note: Make sure whatever this points to doesn't get deallocated before this!
template <typename T>
class ArrayReference {
	T* _start;
	const size_t _size;
public:
	constexpr ArrayReference(): _start(nullptr), _size(0) {}
	constexpr ArrayReference(T* start, size_t size): _start(start), _size(size) {}

	constexpr T* begin() const { return _start; }
	constexpr T* end() const { return _start + _size; }
	constexpr size_t size() const { return _size; }

	constexpr T& operator[](size_t index) const { return _start[index]; }

	constexpr operator ArrayReference<const T>() const { return ArrayReference<const T>(_start, _size); }

	constexpr ArrayReference<T> dropLast(size_t count) {
		return count <= _size ? ArrayReference<T>(_start, _size - count) : ArrayReference<T>(_start, 0);
	}

	constexpr ArrayReference<T> dropFirst(size_t count) {
		return count <= _size ? ArrayReference<T>(_start + count, _size - count) : ArrayReference<T>(_start + _size, 0);
	}
};

class ConstStringReference: public ArrayReference<const char> {
public:
	using ArrayReference::ArrayReference;
	ConstStringReference(): ArrayReference() {}
	explicit ConstStringReference(ArrayReference<const char> array): ArrayReference(array) {}
#ifdef Arduino_h
	ConstStringReference(const String& string): ArrayReference{string.c_str(), string.length()} {}
#else
	ConstStringReference(const std::string& string): ConstArrayReference{string.c_str(), string.size()} {}
#endif
};

class StringReference: public ArrayReference<char> {
public:
	using ArrayReference::ArrayReference;
	StringReference(): ArrayReference() {}
	explicit StringReference(const ArrayReference<char> array): ArrayReference(array) {}
	operator ConstStringReference() const { return ConstStringReference{ArrayReference<const char>{*this}}; }
};

} /* namespace serialctl */

constexpr serialctl::ConstStringReference operator ""_s(const char *str, size_t size) { return {str, size}; }