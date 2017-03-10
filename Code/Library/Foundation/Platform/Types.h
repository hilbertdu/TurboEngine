// Types.h
//------------------------------------------------------------------------------

#pragma once
#ifndef FOUNDATION_PLATFORM_TYPES_H
#define FOUNDATION_PLATFORM_TYPES_H


// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Platform.h"

// Common types
//------------------------------------------------------------------------------
typedef unsigned char 		uint8;		// 8-bit  unsigned.
typedef unsigned short int	uint16;		// 16-bit unsigned.
typedef unsigned int		uint32;		// 32-bit unsigned.
typedef unsigned long long	uint64;		// 64-bit unsigned.

typedef	signed char			int8;		// 8-bit  signed.
typedef signed short int	int16;		// 16-bit signed.
typedef signed int	 		int32;		// 32-bit signed.
typedef signed long long	int64;		// 64-bit signed.

typedef char				CHAR;		// An ANSI character       -                  8-bit fixed-width representation of 7-bit characters.
typedef wchar_t				WCHAR;		// A wide character        - In-memory only.  ?-bit fixed-width representation of the platform's natural wide character set.  Could be different sizes on different platforms.
typedef uint8				CHAR8;		// An 8-bit character type - In-memory only.  8-bit representation.  Should really be char8_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
typedef uint16				CHAR16;		// A 16-bit character type - In-memory only.  16-bit representation.  Should really be char16_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).
typedef uint32				CHAR32;		// A 32-bit character type - In-memory only.  32-bit representation.  Should really be char32_t but making this the generic option is easier for compilers which don't fully support C++11 yet (i.e. MSVC).

typedef size_t				SIZET;


#if defined(__WIN64__)
	typedef int64			INTPTR;
	typedef uint64			UINTPTR;
#elif defined(__WIN32__)
	typedef int32			INTPTR;
	typedef uint32			UINTPTR;
#endif


// Prefix for declaring string and character literals of the default character type.
#define TXT(X)	X


//------------------------------------------------------------------------------
#endif // FOUNDATION_PLATFORM_TYPES_H
