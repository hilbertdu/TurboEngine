// LogManager.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_LOG_MANAGER_H
#define FOUNDATION_LOG_MANAGER_H

// Includes
//-----------------------------------------------------------------------------
#include "Platform/Types.h"
#include "Pattern/Singleton.h"
#include "Logger.h"

class LogManager: public Singleton<LogManager>
{
public:
	LogManager();
	~LogManager();

	void RegisteLogger(const char* tag, uint32 showLevel);
	void UnregisteLogger(const char* tag, uint32 showLevel);
	void GetLogger(const char* tag);

private:

}

#endif // FOUNDATION_LOG_MANAGER_H
//------------------------------------------------------------------------------