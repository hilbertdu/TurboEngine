// CRC32.h
//------------------------------------------------------------------------------
#pragma once
#ifndef CORE_MATH_CRC32_H
#define CORE_MATH_CRC32_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/String/String.h"

// CRC32
//------------------------------------------------------------------------------
class CRC32
{
public:
	static inline uint32	Start() { return 0xFFFFFFFF; }
	static uint32			Update(uint32 crc32, const void * buffer, size_t len);
	static uint32			UpdateLower(uint32 crc32, const void * buffer, size_t len);
	static inline uint32	Stop(uint32 crc32) { return (crc32 ^ 0xFFFFFFFF); }

	static uint32			Calc(const void * buffer, size_t len);
	static uint32			CalcLower(const void * buffer, size_t len);

	inline static uint32	Calc(const AString & string) { return Calc(string.Get(), string.GetLength()); }
	inline static uint32	CalcLower(const AString & string) { return CalcLower(string.Get(), string.GetLength()); }
};

//------------------------------------------------------------------------------
#endif // CORE_MATH_CRC32_H
