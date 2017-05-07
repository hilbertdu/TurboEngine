// Mutex.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_PROCESS_MUTEX_H
#define FOUNDATION_PROCESS_MUTEX_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/Platform/Platform.h"

#if defined(__LINUX__) || defined(__APPLE__)
    #include <pthread.h>
#endif

// Mutex
//------------------------------------------------------------------------------
class Mutex
{
public:
	Mutex();
	~Mutex();

	void Lock();
	void Unlock();

private:
	// do this to avoid including windows.h
	#if defined(__WIN64__)
		unsigned char m_CriticalSection[8 + 4 + 4 + 8 + 8 + 8]; // CRITICAL_SECTION
	#elif defined(__WIN32__)
		unsigned char m_CriticalSection[4 + 4 + 4 + 4 + 4 + 4]; // CRITICAL_SECTION
	#endif
	
    #if defined(__LINUX__) || defined(__APPLE__)
        pthread_mutex_t m_Mutex;
    #endif
};

// MutexHolder
//------------------------------------------------------------------------------
class MutexHolder
{
public:
	MutexHolder(Mutex & mutex): m_Mutex(mutex) { mutex.Lock(); }
	~MutexHolder() { m_Mutex.Unlock(); }

	void operator=(MutexHolder &) = delete;

private:
	Mutex & m_Mutex;
};

//------------------------------------------------------------------------------
#endif // FOUNDATION_PROCESS_MUTEX_H
