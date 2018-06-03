// WorkerThread - object to process and manage jobs on a thread
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_TASK_WORKER_H
#define FOUNDATION_TASK_WORKER_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/Process/Thread.h"
#include <atomic>

// Forward declaration
//------------------------------------------------------------------------------
class TaskScheduler;

// WorkerThread
//------------------------------------------------------------------------------
class WorkerThread
{
public:
	explicit WorkerThread(uint32 threadIndex = -1);
	virtual ~WorkerThread();

	void Init(TaskScheduler * scheduler);
	void WaitForStop();

	inline void Stop() { m_ShouldExit = true; }
	inline bool HasExited() const { return m_Exited; }	

	static uint32 GetThreadIndex();

protected:
	// worker thread main loop
	static uint32	ThreadWrapperFunc(void * param);
	static bool		Update();
	virtual void	Main();	

	// signal to exit thread
	std::atomic<bool> m_ShouldExit;
	std::atomic<bool> m_Exited;

	uint32			m_ThreadIndex;
	TaskScheduler * m_TaskScheduler;
};

//------------------------------------------------------------------------------
#endif // FOUNDATION_TASK_WORKER_H 