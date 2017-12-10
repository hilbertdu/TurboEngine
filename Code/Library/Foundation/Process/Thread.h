// Thread functionality
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_PROCESS_THREAD_H
#define FOUNDATION_PROCESS_THREAD_H


// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include <thread>

// Thread
//------------------------------------------------------------------------------
class Thread
{
public:
	using ThreadId = std::thread::id;
	using ThreadInst = std::thread;
	using ThreadHandle = std::thread::native_handle_type;
	using ThreadEntryFunction = void(*)(void * param);

	static ThreadId GetCurrentThreadId();
	static bool IsThread(ThreadId threadId) { return (GetCurrentThreadId() == threadId); }
	static bool IsMainThread() { return GetCurrentThreadId() == s_MainThreadId; }

	static ThreadInst CreateThread(ThreadEntryFunction entryFunc, void * param);
	static void JoinThread(ThreadInst t);
	static void DetachThread(ThreadInst t);

	static void Sleep(uint32 ms);

private:
	static ThreadId s_MainThreadId;
};

//------------------------------------------------------------------------------
#endif // FOUNDATION_PROCESS_THREAD_H
