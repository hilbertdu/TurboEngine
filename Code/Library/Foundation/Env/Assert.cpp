// Assert.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Env/Assert.h"
#include "Foundation/Platform/Platform.h"
#include "Foundation/String/String.h"

#ifdef ASSERT_ENABLED
	#if defined(__WINDOWS__)
		#include <windows.h>
	#endif
#endif


// IsDebuggerAttached
//------------------------------------------------------------------------------
bool IsDebuggerAttached()
{
#ifdef ASSERT_ENABLED
	#if defined(__WINDOWS__)
		return (IsDebuggerPresent() == TRUE);
	#elif defined(__APPLE__)
		return false; // TODO:MAC Implement IsDebugerAttached
	#else
		#error Unknown platform
	#endif
#else
	return false;
#endif
}


// Static
//------------------------------------------------------------------------------
#ifdef ASSERT_HANDLER
/*static*/ bool AssertHandler::s_ThrowOnAssert(false);
#endif


// FailureM
//------------------------------------------------------------------------------
#ifdef ASSERT_HANDLER
/*static*/ bool AssertHandler::Failure(const char * message,
	const char * file,
	const int line,
	const char * fmtString,
	...)
{
	AStackString<4096> buffer(message);
	buffer += '\n';

	AStackString<4096> buffer2;
	va_list args;
	va_start(args, fmtString);
	buffer2.VFormat(fmtString, args);
	va_end(args);

	buffer += buffer2;
	return true; // break execution
}
#endif

//------------------------------------------------------------------------------
