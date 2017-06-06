// Logger.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_LOGGER_H
#define FOUNDATION_LOGGER_H

// Logger usage
//================================
// LOG("This is a simple log");
// LOG("This is a format log: %d", 100);
// LOG(Memory, "This is a memory log: %s", 1000);
//================================

// Includes
//-----------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include <stdarg.h>

// Macros
//-----------------------------------------------------------------------------
#define LOGLEVEL_DEBUG			(0x0001)
#define LOGLEVEL_VERBOSE		(0x0003)
#define LOGLEVEL_INFO			(0x0007)
#define LOGLEVEL_WARN			(0x000F)
#define LOGLEVEL_ERROR			(0x001F)
#define LOGLEVEL_FATALERROR		(0x003F)

#define _LOG(tag, slevel, level, ...)				\
	do {											\
	PRAGMA_DISABLE_PUSH_MSVC(4127)					\
		if (level & Logger::GetLoggerLevel(#tag))	\
		{											\
			Logger::LogTag(slevel, __VA_ARGS__);	\
		}											\
	} while (0)										\
	PRAGMA_DISABLE_POP_MSVC


// Expose interface
//----------------------------------------------------------------------------
#define LDEBUG(tag, ...)		_LOG(tag, "DEBUG", LOGLEVEL_DEBUG, __VA_ARGS__)
#define LVERBOSE(tag, ...)		_LOG(tag, "VERBOSE", LOGLEVEL_VERBOSE, __VA_ARGS__)
#define LINFO(tag, ...)			_LOG(tag, "INFO", LOGLEVEL_VERBOSE, __VA_ARGS__)
#define LWARN(tag, ...)			_LOG(tag, "WARN", LOGLEVEL_VERBOSE, __VA_ARGS__)
#define LERROR(tag, ...)		_LOG(tag, "ERROR", LOGLEVEL_VERBOSE, __VA_ARGS__)
#define LFATALERROR(tag, ...)	_LOG(tag, "FATALERROR", LOGLEVEL_VERBOSE, __VA_ARGS__)

// Directly output
#define LOUTPUT(...)			Logger::Log(__VA_ARGS__)


// Logger
//------------------------------------------------------------------------------
class Logger
{
public:
	static void RegisteLogger(const char * tag, int32 showLevel);
	static void UnregisteLogger(const char * tag);
	static int32 GetLoggerLevel(const char* tag);

	static inline void DoNothing() {}

	static void Log(const char * format, ...);
	static void LogTag(const char * tag, const char * format, ...);

private:
	static void Output(const char* format, va_list args);
	static void OutputDirect(const char * msg);
};


#endif // FOUNDATION_LOGGER_H
//------------------------------------------------------------------------------