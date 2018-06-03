// Command.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Command.h"
#include "../Engine/EngineCore.h"
#include "../Event/EventDecl.h"
#include "Foundation/Process/Thread.h"
#include "Foundation/Process/Process.h"


// Run
//------------------------------------------------------------------------------
/*virtual*/ uint32 ICommand::Run()
{
	if (!m_Task.Get() || m_Task->IsFinished() || m_Task->IsCancelled())
	{
		m_Task = TNEW(ExeCommandTask(m_Executable, m_Parameters, m_WorkingDir, m_Environment));
		EngineCore::Instance().GetTaskScheduler()->AddTask(m_Task);
		return 0;
	}
	return 1;
}

// Cancel
//------------------------------------------------------------------------------
/*virtual*/ uint32 ICommand::Cancel()
{
	if (!m_Task.Get())
	{
		EngineCore::Instance().GetTaskScheduler()->CancelTask(m_Task);
		m_Task = nullptr;
		return 0;
	}
	return 1;
}

// Constructor
//------------------------------------------------------------------------------
ExeCommandTask::ExeCommandTask(const AStringView & exe, const AStringView & args, const AStringView & dir, const AStringView & env)
	: m_Executable(exe.Get())
	, m_Arguments(args.Get())
	, m_WorkingDir(dir.Get())
	, m_Environment(env.Get())
{
}

// Execute
//------------------------------------------------------------------------------
/*virtual*/ uint32 ExeCommandTask::Excute()
{
	bool spawnOK = m_Process.Spawn(m_Executable.Get(),
		m_Arguments.IsEmpty() ? nullptr : m_Arguments.Get(), 
		m_WorkingDir.IsEmpty() ? nullptr : m_WorkingDir.Get(),
		m_Environment.IsEmpty() ? nullptr : m_Environment.Get());

	if (!spawnOK)
	{
		LERROR("Command", "Failed to spwan process '%s', (%d)\n", m_Executable.Get(), Platform::GetLastErrorNo());
		return false;
	}

	uint32 bufferSize = MEGABYTE;
	uint32 readSize = -1;
	char *output = TNEW(char[bufferSize]());
	for (; m_Process.IsRunning() || readSize != 0;)
	{
		readSize = m_Process.ReadStdOut(output, bufferSize - 1);
		output[readSize] = '\000';
		LOUTPUT(output);
		m_OutputLock.Lock();
		m_Output += output;
		m_OutputLock.UnLock();
		Thread::Sleep(15);
	}
	TDELETE_SAFE(output);

	ASSERT(!m_Process.IsRunning());
	return m_Process.WaitForExit();
}

// Cancel
//------------------------------------------------------------------------------
/*virtual*/ uint32 ExeCommandTask::Cancel()
{
	return -1;
}

// FetchOutput
//------------------------------------------------------------------------------
const AStringView ExeCommandTask::FetchOutput(uint32 index, uint32 len) const
{
	ASSERT(index <= m_Output.GetLength());
	SIZET realLen = m_Output.GetLength() >= index + len ? len : m_Output.GetLength() - index;
	return AStringView(m_Output.Get() + index, realLen);
}


//------------------------------------------------------------------------------