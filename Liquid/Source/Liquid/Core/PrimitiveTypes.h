#pragma once

#include <cstdint>

namespace Liquid {

	// Signed types
	typedef int8_t int8;
	typedef int16_t int16;
	typedef int32_t int32;
	typedef int64_t int64;
	typedef intptr_t intptr;

	// Unsigned types
	typedef uint8_t uint8;
	typedef uint16_t uint16;
	typedef uint32_t uint32;
	typedef uint64_t uint64;
	typedef uintptr_t uintptr;

	// Test types
	static_assert(sizeof(int8)   == 1 && sizeof(uint8)   == 1);
	static_assert(sizeof(int16)  == 2 && sizeof(uint16)  == 2);
	static_assert(sizeof(int32)  == 4 && sizeof(uint32)  == 4);
	static_assert(sizeof(int64)  == 8 && sizeof(uint64)  == 8);
	static_assert(sizeof(intptr) == 8 && sizeof(uintptr) == 8);

}