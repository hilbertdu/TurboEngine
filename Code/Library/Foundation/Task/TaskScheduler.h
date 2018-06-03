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
#include "Foundation/Container/SmartPtr.h"


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

	void InitWorker(uint16 count = 1);		// TODO: pass worker in

	void Start() {}		// TODO: to be implement
	void Clear() {}		// TODO: to be implement

	void AddTask(const StrongPtr<Task> & task);
	void AddTasks(const Array<StrongPtr<Task>> & task);

	void CancelTask(const StrongPtr<Task> & task);

	bool PollStaging(uint8 count = 1);
	bool PollCompleted(uint8 count = 1);
	bool PollCancelled(uint8 count = 1);

private:
	Array<StrongPtr<Task>> m_StagingTasks;
	Array<StrongPtr<Task>> m_CompletedTasks;
	Array<StrongPtr<Task>> m_CancelledTasks;

	Array<StrongPtr<WorkerThread>> m_Workers;
	SpinLock m_Lock;
};


//------------------------------------------------------------------------------
#endif // FOUNDATION_PROCESS_TASK_TASKSCHEDULER_H