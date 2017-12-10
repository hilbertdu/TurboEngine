// Thread
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Process/Thread.h"
#include "Foundation/Env/Assert.h"
#include "Foundation/Memory/Mem.h"
#include "Foundation/Analyzer/Profile.h"


// Static Data
//------------------------------------------------------------------------------
/*static*/ Thread::ThreadId Thread::s_MainThreadId(Thread::GetCurrentThreadId());

// GetCurrentThreadId
//------------------------------------------------------------------------------
/*static*/ Thread::ThreadId Thread::GetCurrentThreadId()
{
	return std::this_thread::get_id();
}

//  Sleep
//------------------------------------------------------------------------------
/*static*/ void Thread::Sleep(uint32 ms)
{
	return std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

// CreateThread
//------------------------------------------------------------------------------
/*static*/ Thread::ThreadInst Thread::CreateThread(ThreadEntryFunction entryFunc, void * param)
{
	return std::thread(entryFunc, param);
}

// WaitForThread
//------------------------------------------------------------------------------
/*static*/ void Thread::JoinThread(ThreadInst t)
{
	t.join();
}

// CloseHandle
//------------------------------------------------------------------------------
/*static*/ void Thread::DetachThread(ThreadInst t)
{
	t.detach();
}

//------------------------------------------------------------------------------
