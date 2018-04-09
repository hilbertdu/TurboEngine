// TaskScheduler
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Task/TaskScheduler.h"
#include "Foundation/Task/Task.h"
#include "Foundation/Task/Worker.h"


// Constructor
//------------------------------------------------------------------------------
TaskScheduler::TaskScheduler()
{}

// Destructor
//------------------------------------------------------------------------------
TaskScheduler::~TaskScheduler()
{
	for (auto item : m_Workers) { TDELETE_SAFE(item); }
	for (auto item : m_TaskQueue) { TDELETE_SAFE(item); }	
}

// InitWorker
//------------------------------------------------------------------------------
void TaskScheduler::InitWorker(uint16 count /* = 1 */)
{
	for (uint32 idx = 0; idx < count; ++idx)
	{
		WorkerThread * worker = TNEW(WorkerThread);
		m_Workers.Append(worker);
		worker->Init(this);
	}
}

// PushTask
//------------------------------------------------------------------------------
void TaskScheduler::PushTask(Task * task)
{
	LockGuard<SpinLock> mh(m_Lock);

	m_TaskQueue.Append(task);
}

// PushTasks
//------------------------------------------------------------------------------
void TaskScheduler::PushTasks(Array<Task*> task)
{
	LockGuard<SpinLock> mh(m_Lock);

	m_TaskQueue.Append(task);
}

// PopTask
//------------------------------------------------------------------------------
Task * TaskScheduler::PopTask()
{
	LockGuard<SpinLock> mh(m_Lock);

	if (m_TaskQueue.IsEmpty())
	{
		return nullptr;
	}

	Task * task = m_TaskQueue.TopItem();
	m_TaskQueue.PopFront();

	return task;
}

//------------------------------------------------------------------------------