// Task.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_PROCESS_TASK_TASKSCHEDULER_H
#define FOUNDATION_PROCESS_TASK_TASKSCHEDULER_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Container/Array.h"
#include "Foundation/Process/Thread.h"
#include "Foundation/Process/Lock.h"


// Forward declaration
//------------------------------------------------------------------------------
class Task;
class WorkerThread;


// Class TaskScheduler
//------------------------------------------------------------------------------
class TaskScheduler
{
public:
	TaskScheduler();
	~TaskScheduler();

	void   InitWorker(uint16 count = 1);

	void   Start() {}		// TODO: to be implement
	void   Pause() {}		// TODO: to be implement
	void   Resume() {}		// TODO: to be implement
	void   Clear() {}		// TODO: to be implement

	void   PushTask(Task * task);
	void   PushTasks(Array<Task*> task);
	Task * PopTask();

private:
	Array<Task*> m_TaskQueue;
	Array<WorkerThread*> m_Workers;
	SpinLock m_Lock;
};


//------------------------------------------------------------------------------
#endif // FOUNDATION_PROCESS_TASK_TASKSCHEDULER_H