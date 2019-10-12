#pragma once

#include "std/type_traits"

namespace serialctl {

template <bool _Bp, class _Tp = void> using enable_if_t = typename std::enable_if<_Bp, _Tp>::type;

#pragma mark - all_same
// From https://stackoverflow.com/a/31185980
template <typename... T> struct all_same   : std::false_type {};
template <>              struct all_same<> : std::true_type  {};
template <typename T>    struct all_same<T>: std::true_type  {};
template <typename T, typename... Ts> struct all_same<T, T, Ts...>: all_same<T, Ts...> {};

#pragma mark - UIntLargeEnoughFor
template <size_t Num>
struct _UIntLargeEnoughFor {
	using type = typename std::conditional<Num <= 0xffff,
		typename std::conditional<Num <= 0xff, uint8_t, uint16_t>::type,
		typename std::conditional<Num <= 0xffffffff, uint32_t, uint64_t>::type
	>::type;
};

template <size_t Num> using UIntLargeEnoughFor = typename _UIntLargeEnoughFor<Num>::type;

}

#include <Arduino.h>

static inline uint8_t pgm_read(const uint8_t *address) {
	return pgm_read_byte((uintptr_t)address);
}

static inline uint16_t pgm_read(const uint16_t *address) {
	return pgm_read_word((uintptr_t)address);
}

static inline uint32_t pgm_read(const uint32_t *address) {
	return pgm_read_dword((uintptr_t)address);
}

static inline float pgm_read(const float *address) {
	return pgm_read_float((uintptr_t)address);
}