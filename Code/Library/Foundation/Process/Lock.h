// Lock.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_PROCESS_LOCK_H
#define FOUNDATION_PROCESS_LOCK_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/Platform/Platform.h"
#include <atomic>


// Class MetexLock
//------------------------------------------------------------------------------
class MutexLock
{
public:
	MutexLock();
	~MutexLock();

	void Lock();
	bool TryLock();
	void UnLock();

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


// Class SpinLock (simple use atomic_flag)
//------------------------------------------------------------------------------
class SpinLock
{
public:
	void Lock();
	bool TryLock();
	void UnLock();

private:
	std::atomic_flag m_Lock = ATOMIC_FLAG_INIT;
};


// Class LockGuard
//------------------------------------------------------------------------------
template<class Lock>
class LockGuard
{
public:
	LockGuard(Lock & lock) : m_Lock(lock) { m_Lock.Lock(); }
	~LockGuard() { m_Lock.UnLock(); }

	void operator = (LockGuard &) = delete;

private:
	Lock & m_Lock;
};

//------------------------------------------------------------------------------
#endif // FOUNDATION_PROCESS_LOCK_H
