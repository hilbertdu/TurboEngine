// Lock.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Process/Lock.h"
#include "Foundation/Env/Assert.h"

#if defined(__WINDOWS__)
    #include <windows.h>
#endif
#if defined(__LINUX__) || defined(__APPLE__)
    #include <pthread.h>
#endif

// CONSTRUCTOR
//------------------------------------------------------------------------------
MutexLock::MutexLock()
{ 
#if defined(__WINDOWS__)
	CASSERT(sizeof(m_CriticalSection) == sizeof(CRITICAL_SECTION));
	VERIFY(InitializeCriticalSectionAndSpinCount((CRITICAL_SECTION *)&m_CriticalSection, 100));
#elif defined(__LINUX__) || defined(__APPLE__)
	pthread_mutexattr_t attributes;
	VERIFY(pthread_mutexattr_init(&attributes) == 0);
	pthread_mutexattr_settype(&attributes, PTHREAD_MUTEX_RECURSIVE);
	VERIFY(pthread_mutex_init(&m_Mutex, &attributes) == 0);
#else
	#error Unknown platform
#endif
}

// DESTRUCTOR
//------------------------------------------------------------------------------
MutexLock::~MutexLock()
{ 
#if defined(__WINDOWS__)
    DeleteCriticalSection((CRITICAL_SECTION *)&m_CriticalSection);
#elif defined(__LINUX__) || defined(__APPLE__)
    VERIFY(pthread_mutex_destroy(&m_Mutex) == 0);
#else
    #error Unknown platform
#endif
}

PRAGMA_DISABLE_PUSH_MSVC(26135) // static analysis complains about missing annotation
// Lock
//------------------------------------------------------------------------------
void MutexLock::Lock()
{ 
#if defined(__WINDOWS__)
	EnterCriticalSection((CRITICAL_SECTION *)&m_CriticalSection);
#elif defined(__LINUX__) || defined(__APPLE__)
	VERIFY(pthread_mutex_lock(&m_Mutex) == 0);
#else
	#error Unknown platform
#endif
}
// TryLock
//------------------------------------------------------------------------------
bool MutexLock::TryLock()
{
#if defined(__WINDOWS__)
	return (bool)TryEnterCriticalSection((CRITICAL_SECTION *)&m_CriticalSection);
#elif defined(__LINUX__) || defined(__APPLE__)
	VERIFY(pthread_mutex_lock(&m_Mutex) == 0);
#else
	#error Unknown platform
#endif
}
// Unlock
//------------------------------------------------------------------------------
void MutexLock::UnLock()
{
#if defined(__WINDOWS__)
	LeaveCriticalSection((CRITICAL_SECTION *)&m_CriticalSection);
#elif defined(__LINUX__) || defined(__APPLE__)
	VERIFY(pthread_mutex_unlock(&m_Mutex) == 0);
#else
	#error Unknown platform
#endif
}
PRAGMA_DISABLE_POP_MSVC


// Lock
//------------------------------------------------------------------------------
void SpinLock::Lock()
{
	while (m_Lock.test_and_set(std::memory_order_acquire)) {}
}

// TryLock
//------------------------------------------------------------------------------
bool SpinLock::TryLock()
{
	return m_Lock.test_and_set(std::memory_order_acquire);
}

// Unlock
//------------------------------------------------------------------------------
void SpinLock::UnLock()
{
	m_Lock.clear(std::memory_order_release);
}

//------------------------------------------------------------------------------
