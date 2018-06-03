// Task.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_PROCESS_TASK_TASK_H
#define FOUNDATION_PROCESS_TASK_TASK_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/Container/Array.h"
#include <atomic>


// Class Task
//------------------------------------------------------------------------------
class Task
{
public:
	enum class Status : uint8
	{
		STATUS_INVALID		= 0,
		STATUS_READY		= 1,
		STATUS_RUNNING		= 2,
		STATUS_SUSPEND		= 3,
		STATUS_OK			= 4,
		STATUS_FAILED		= 5,
		STATUS_CANCELLED	= 6,
	};

	explicit Task();
	virtual ~Task();

	Task(const Task & task) = delete;

	inline void		SetStatus(Status statu) { m_Status = statu; }
	inline Status	GetStatus() const { return m_Status; }
	inline bool		IsFinished() const { return m_Status == Status::STATUS_OK || m_Status == Status::STATUS_FAILED; }
	inline bool		IsCancelled() const { return m_Status == Status::STATUS_CANCELLED; }
	inline bool		IsValid() const { return m_Status != Status::STATUS_INVALID; }

	inline bool		AddChildren(Task * task) { task->m_Deps.Append(this); m_Chidren.Append(task); }

protected:
	virtual uint32 Excute()	= 0;
	virtual uint32 Cancel()	= 0;

	virtual void OnStarted() {}
	virtual void OnFinished() {}
	virtual void OnCancelled() {}

	uint32  m_TaskId;
	Status  m_Status;

	Array<Task *> m_Deps;
	Array<Task *> m_Chidren;

	friend class TaskScheduler;
};


// MakeTask
//------------------------------------------------------------------------------
// TODO

//------------------------------------------------------------------------------
#endif // FOUNDATION_PROCESS_TASK_TASK_H