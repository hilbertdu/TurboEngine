// Worker.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Task/Worker.h"
#include "Foundation/Task/Task.h"
#include "Foundation/Task/TaskScheduler.h"
#include "Foundation/Process/Thread.h"
#include "Foundation/Platform/Misc.h"
#include "Foundation/Logging/Logger.h"

// Static
//------------------------------------------------------------------------------
static THREAD_LOCAL uint32 s_WorkerThreadThreadIndex = 0;
static THREAD_LOCAL TaskScheduler * s_TaskScheduler = nullptr;
static volatile uint32 s_ThreadFreeIndex = 0;


// Constructor
//------------------------------------------------------------------------------
WorkerThread::WorkerThread(uint32 threadIndex)
	: m_ShouldExit(false)
	, m_Exited(false)
{
	if (threadIndex == -1)
	{
		m_ThreadIndex = ++s_ThreadFreeIndex;
	}
}

// Destructor
//------------------------------------------------------------------------------
WorkerThread::~WorkerThread()
{
	m_ShouldExit = true;
	WaitForStop();
}

void WorkerThread::Init(TaskScheduler * scheduler)
{
	// Start thread
	m_TaskScheduler = scheduler;
	Thread::CreateThread(ThreadWrapperFunc, "WorkerThread", 64 * KILOBYTE, this);
}

void WorkerThread::WaitForStop()
{
	while (m_Exited == false)
	{
		Thread::Sleep(1);
	}
}

/*static*/ uint32 WorkerThread::ThreadWrapperFunc(void * param)
{
	WorkerThread * wt = reinterpret_cast<WorkerThread*>(param);
	s_WorkerThreadThreadIndex = wt->m_ThreadIndex;
	s_TaskScheduler = wt->m_TaskScheduler;
	wt->Main();
	return 0;
}

/*static*/ uint32 WorkerThread::GetThreadIndex()
{
	return s_WorkerThreadThreadIndex;
}

/*virtual*/ void WorkerThread::Main()
{
	while (m_ShouldExit == false)
	{
		bool didSomeWork = Update();
		if (!didSomeWork)
		{
			Thread::Sleep(1); // no work to do right now, wait and try again later
		}
	}

	m_Exited = true;
}

/*static*/ bool WorkerThread::Update()
{
	return s_TaskScheduler->PollStaging();
}