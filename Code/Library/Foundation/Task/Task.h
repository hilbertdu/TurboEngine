// Task.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_PROCESS_TASK_TASK_H
#define FOUNDATION_PROCESS_TASK_TASK_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include <atomic>


// Task
//------------------------------------------------------------------------------
class Task
{
public:
	enum class Status : uint8
	{
		STATUS_INVALID	= 0,
		STATUS_SUSPEND	= 1,
		STATUS_RUNNING	= 2,
		STATUS_OK		= 3,
		STATUS_FAILED	= 4
	};

	explicit Task();
	virtual ~Task();

	Task(const Task & task) = delete;

	virtual uint32 Run() = 0;
	virtual uint32 Cancel() = 0;

	inline void		SetStatus(Status statu) { m_Status = statu; }
	inline Status	GetStatus() const { return m_Status; }
	inline bool		IsFinished() const { return m_Status == Status::STATUS_OK || m_Status == Status::STATUS_FAILED; }

protected:
	uint32  m_TaskId;
	Status  m_Status;
	Task *  m_Parent;
};


//------------------------------------------------------------------------------
#endif // FOUNDATION_PROCESS_TASK_TASK_H