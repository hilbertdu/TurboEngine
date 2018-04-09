// Assert.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_ENV_ASSERT_H
#define FOUNDATION_ENV_ASSERT_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Platform.h"
#include "Foundation/Platform/Misc.h"

// Global functions
//------------------------------------------------------------------------------
bool IsDebuggerAttached();

// Defines
//------------------------------------------------------------------------------
#define ASSERT_ENABLED

#if defined(__WINDOWS__)
	#define BREAK_IN_DEBUGGER __debugbreak();
#elif defined(__APPLE__)
	#define BREAK_IN_DEBUGGER __builtin_trap();
#else
	#error Unknown platform
#endif


// ASSERT
//------------------------------------------------------------------------------
#ifdef ASSERT_ENABLED
	#define ASSERT_HANDLER
	#define ASSERT	ASSERT_REAL
	#define CASSERT	CASSERT_REAL
	#define VERIFY	VARIFY_REAL
#else
	#define ASSERT	ASSERT_NONE
	#define CASSERT	CASSERT_NONE
	#define VERIFY	VARIFY_NONE
#endif

// ASSERT define
//-----------------------------------------------------------------------------
#define ASSERT_REAL(expression, ...)													\
	do {																				\
		PRAGMA_DISABLE_PUSH_MSVC(4127)													\
		if (!(expression))																\
		{																				\
			if (AssertHandler::FailureM(#expression, __FILE__, __LINE__, __VA_ARGS__))	\
			{																			\
				BREAK_IN_DEBUGGER;														\
			}																			\
		}																				\
	} while (0);																		\
	PRAGMA_DISABLE_POP_MSVC

#define ASSERT_NONE(expression) // Ignore


// CASSERT define
//-----------------------------------------------------------------------------
#define CASSERT_REAL(expression) static_assert(expression,	\
"==>COMPILE TIME ASSERTION FAILURE\n"						\
"==>Expression: " #expression "\n")

#define CASSERT_NONE(expression) // Ignore


// VERIFY define
//-----------------------------------------------------------------------------
// assert result of code, but still execute code when asserts are disabled
#define VARIFY_REAL(code) ASSERT_REAL(code)
#define VARIFY_NONE(code) code


// AssertHandler
//-----------------------------------------------------------------------------
#ifdef ASSERT_HANDLER
	class AssertHandler										\
	{														\
	public:													\
		static void SetThrowOnAssert(bool throwOnAssert)	\
		{													\
			s_ThrowOnAssert = throwOnAssert;				\
		}													\
		static bool Failure(const char * message,			\
			const char * file,								\
			const int line);								\
		static bool FailureM(const char * message,			\
			const char * file,								\
			const int line,									\
			const char * msgFormat = "",					\
			...);											\
															\
		static bool s_ThrowOnAssert;						\
	};
#endif

//------------------------------------------------------------------------------
#endif // FOUNDATION_ENV_ASSERT_H
