#pragma once

#include "Utility.hpp"
#include "ArrayReference.hpp"

namespace serialctl {

template <typename T, size_t Capacity>
class SmallVector {
protected:
	T _data[Capacity];
public:
	using SizeType = UIntLargeEnoughFor<Capacity>;
protected:
	SizeType _size;
public:
	SizeType size() const { return _size; }
	SizeType capacity() const { return Capacity; }

	T* data() { return _data; }
	const T* data() const { return _data; }

	SmallVector(): _size(0) {}

	template <typename... Ts, typename = enable_if_t<sizeof...(Ts) <= Capacity>, typename = enable_if_t<all_same<T, Ts...>::value>>
	explicit SmallVector(Ts... arr) : _data{arr...}, _size(sizeof...(arr)) {}

	explicit SmallVector(ArrayReference<const T> array): _size(0) {
		extend(array);
	}

	static SmallVector repeating(const T& element, SizeType count = Capacity) {
		SmallVector arr;
		arr._size = count <= Capacity ? count : Capacity;
		for (T& item : arr) {
			item = element;
		}
		return arr;
	}

	T* begin() { return _data; }
	const T* begin() const { return _data; }

	T* end() { return _data + _size; }
	const T* end() const { return _data + _size; }

	T& operator[](size_t index) { return _data[index]; }
	const T& operator[](size_t index) const { return _data[index]; }

	void append(T element) {
		if (_size < Capacity) {
			_data[_size++] = move(element);
		}
	}

	void extend(ArrayReference<T> array) {
		for (const T* item = array.begin(), end = array.end(); _size < Capacity && item < end; item++) {
			append(*item);
		}
	}

	T pop() {
		if (_size == 0) { return T(); }
		_size -= 1;
		return move(_data[_size]);
	}

	void clear() {
		for (T& item : *this) {
			item.~T();
		}
		_size = 0;
	}

	operator ArrayReference<T>() { return {_data, _size}; }
	operator ArrayReference<const T>() const { return {_data, _size}; }
};

} /* namespace serialctl */
