// Misc.h
//------------------------------------------------------------------------------

#pragma once
#ifndef FOUNDATION_PLATFORM_MISC_H
#define FOUNDATION_PLATFORM_MISC_H


// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Platform.h"


// Warning disabling
//------------------------------------------------------------------------------
#if defined(__WINDOWS__)
	#define PRAGMA_DISABLE_PUSH_MSVC(num)	__pragma(warning(push))			\
											__pragma(warning(disable:num))
	#define PRAGMA_DISABLE_POP_MSVC         __pragma(warning(pop))
#else
	// Do nothing on non-msvc platforms
	#define PRAGMA_DISABLE_PUSH_MSVC(num)
	#define PRAGMA_DISABLE_POP_MSVC
#endif


// Force inline
//------------------------------------------------------------------------------
#if defined(__WINDOWS__)
	#define NO_INLINE		__declspec( noinline )
	#define FORCE_INLINE	__forceinline
#elif defined(__APPLE__)
	#define NO_INLINE		inline __attribute__((noinline))
	#define FORCE_INLINE	inline __attribute__((always_inline))
#endif


//------------------------------------------------------------------------------
#define KILOBYTE (1024)
#define MEGABYTE (1024 * 1024)

#define UNUSED( x )

//------------------------------------------------------------------------------
#endif // FOUNDATION_PLATFORM_MISC_H