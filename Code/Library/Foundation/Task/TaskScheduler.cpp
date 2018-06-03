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
}

// InitWorker
//------------------------------------------------------------------------------
void TaskScheduler::InitWorker(uint16 count /* = 1 */)
{
	for (uint32 idx = 0; idx < count; ++idx)
	{
		StrongPtr<WorkerThread> worker(TNEW(WorkerThread));
		m_Workers.Append(worker);
		worker->Init(this);
	}
}

// PushTask
//------------------------------------------------------------------------------
void TaskScheduler::AddTask(const StrongPtr<Task> & task)
{
	LockGuard<SpinLock> mh(m_Lock);

	m_StagingTasks.Append(task);
}

// PushTasks
//------------------------------------------------------------------------------
void TaskScheduler::AddTasks(const Array<StrongPtr<Task>> & tasks)
{
	LockGuard<SpinLock> mh(m_Lock);

	m_StagingTasks.Append(tasks);
}

// CancelTask
//------------------------------------------------------------------------------
void TaskScheduler::CancelTask(const StrongPtr<Task> & task)
{
	task->SetStatus(Task::Status::STATUS_CANCELLED);
}

// PollStaging
//------------------------------------------------------------------------------
bool TaskScheduler::PollStaging(uint8 count)
{
	while (count-- > 0)
	{
		StrongPtr<Task> stagingTask;
		{
			LockGuard<SpinLock> mh(m_Lock);

			if (m_StagingTasks.IsEmpty())
			{
				return false;
			}

			stagingTask = m_StagingTasks.TopItem();
			m_StagingTasks.PopFront();
		}

		LDEBUG("TaskScheduler", "Task on started\n");
		stagingTask->SetStatus(Task::Status::STATUS_RUNNING);
		stagingTask->OnStarted();
		uint32 result = stagingTask->Excute();
		stagingTask->SetStatus(result == 0 ? Task::Status::STATUS_OK : Task::Status::STATUS_FAILED);
	}
	return true;
}

// PollFinished
//------------------------------------------------------------------------------
bool TaskScheduler::PollCompleted(uint8 count)
{
	while (count-- > 0)
	{
		StrongPtr<Task> completedTask;
		{
			LockGuard<SpinLock> mh(m_Lock);

			for (auto iter = m_StagingTasks.Begin(); iter != m_StagingTasks.End(); ++iter)
			{
				if ((*iter)->IsFinished())
				{
					m_CompletedTasks.Append((*iter));
				}
			}

			if (m_CompletedTasks.IsEmpty())
			{
				return false;
			}

			completedTask = m_CompletedTasks.TopItem();
			m_CompletedTasks.PopFront();
		}
		LDEBUG("TaskScheduler", "Task on finished\n");
		completedTask->OnFinished();
		completedTask->SetStatus(Task::Status::STATUS_INVALID);
	}
	return true;
}

// PollCancelled
//------------------------------------------------------------------------------
bool TaskScheduler::PollCancelled(uint8 count)
{
	while (count-- > 0)
	{
		StrongPtr<Task> cancelledTask;
		{
			LockGuard<SpinLock> mh(m_Lock);

			for (auto iter = m_StagingTasks.Begin(); iter != m_StagingTasks.End(); ++iter)
			{
				if ((*iter)->IsCancelled())
				{
					m_CancelledTasks.Append((*iter));
				}
			}

			if (m_CancelledTasks.IsEmpty())
			{
				return false;
			}

			cancelledTask = m_CancelledTasks.TopItem();
			m_CancelledTasks.PopFront();
		}
		LDEBUG("TaskScheduler", "Task on cancelled\n");
		cancelledTask->OnCancelled();		
	}
	return true;
}

//------------------------------------------------------------------------------