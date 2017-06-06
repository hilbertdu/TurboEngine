// Logger.cpp
//------------------------------------------------------------------------------
#include "Foundation/Logging/Logger.h"
#include "Foundation/String/String.h"
#include "Foundation/Platform/Platform.h"

#include <stdio.h>
#include <stdarg.h>
#if defined(__WINDOWS__) && defined(__DEBUG__)
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

/*static*/ void Logger::Log(const char * format, ...)
{
	va_list args;
	va_start(args, format);
	Output(format, args);
	va_end(args);
}

/*static*/ void Logger::LogTag(const char * tag, const char * format, ...)
{
	va_list args;
	va_start(args, format);
	OutputDirect(tag);
	OutputDirect(" ");
	Output(format, args);
	va_end(args);
}

/*static*/ void Logger::Output(const char* format, va_list args)
{
	AStackString<8192> buffer;

	buffer.VFormat(format, args);

	// normal output that goes to the TTY
	fputs(buffer.Get(), stdout);

	// emit to the debugger as well if possible
#if defined(__WINDOWS__) && defined(__DEBUG__)
	OutputDebugStringA(buffer.Get());
#endif
}

/*static*/ void Logger::OutputDirect(const char * msg)
{
	// normal output that goes to the TTY
	fputs(msg, stdout);

	// emit to the debugger as well if possible
#if defined(__WINDOWS__) && defined(__DEBUG__)
	OutputDebugStringA(msg);
#endif
}

