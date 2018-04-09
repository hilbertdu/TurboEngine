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
	const int line)
{
	const uint32 BUFFER_SIZE(4096);
	char buffer[BUFFER_SIZE];
#if defined(__APPLE__) || defined(__LINUX__)
	sprintf(buffer,
#else
	sprintf_s(buffer, BUFFER_SIZE,
#endif
		"\n-------- ASSERTION FAILED --------\n%s(%i): Assert: %s\n-----^^^ ASSERTION FAILED ^^^-----\n",
		file, line, message);

	puts(buffer);
#if defined(__WINDOWS__)
	OutputDebugStringA(buffer);
#endif

	if (s_ThrowOnAssert)
	{
		throw "AssertionFailed";
	}

	if (IsDebuggerAttached() == false)
	{
#if defined(__WINDOWS__)
		// TODO:LINUX Fix MessageBox use
		// TODO:OSX Fix MessageBox use
		int res = MessageBoxA(nullptr, buffer, "Assertion Failed - Break Execution?", MB_YESNO | MB_ICONERROR);
		return (res == IDYES);
#endif
	}
	return true; // break execution
}

/*static*/ bool AssertHandler::FailureM(const char * message,
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

	return Failure(buffer.Get(), file, line);
}
#endif

//------------------------------------------------------------------------------
