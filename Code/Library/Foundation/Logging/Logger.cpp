// Logger.cpp
//------------------------------------------------------------------------------
#include "Foundation/Logging/Logger.h"
#include "Foundation/String/String.h"
#include "Foundation/Platform/Platform.h"

#include <stdio.h>
#include <stdarg.h>
#if defined(__WINDOWS__) && defined(_DEBUG_)
	#include <windows.h> // for OutputDebugStringA
#endif


/*static*/ void Logger::RegisteLogger(const char * tag, int32 showLevel)
{
}

/*static*/ void Logger::UnregisteLogger(const char * tag)
{
}

/*static*/ int32 Logger::GetLoggerLevel(const char* tag)
{
	return LOGLEVEL_DEBUG;
}

/*static*/ void Logger::Log(const char * fmtString, ...)
{
	va_list args;
	va_start(args, fmtString);
	Output(fmtString, args);
	va_end(args);
}

/*static*/ void Logger::LogDebug(const char * fmtString, ...)
{
	va_list args;
	va_start(args, fmtString);
	Output(fmtString, args);
	va_end(args);
}

/*static*/ void Logger::LogVerbose(const char * fmtString, ...)
{
	va_list args;
	va_start(args, fmtString);
	Output(fmtString, args);
	va_end(args);
}

/*static*/ void Logger::LogInfo(const char * fmtString, ...)
{
	va_list args;
	va_start(args, fmtString);
	Output(fmtString, args);
	va_end(args);
}

/*static*/ void Logger::LogWarn(const char * fmtString, ...)
{
	va_list args;
	va_start(args, fmtString);
	Output(fmtString, args);
	va_end(args);
}

/*static*/ void Logger::LogError(const char * fmtString, ...)
{
	va_list args;
	va_start(args, fmtString);
	Output(fmtString, args);
	va_end(args);
}

/*static*/ void Logger::FatalError(const char * fmtString, ...)
{
	va_list args;
	va_start(args, fmtString);
	Output(fmtString, args);
	va_end(args);
}

/*static*/ void Logger::Output(const char* fmtString, va_list args)
{
	AStackString<8192> buffer;

	buffer.VFormat(fmtString, args);

	// normal output that goes to the TTY
	fputs(buffer.Get(), stdout);

	// emit to the debugger as well if possible
#if defined(__WINDOWS__) && defined(_DEBUG_)
	OutputDebugStringA(buffer.Get());
#endif
}

