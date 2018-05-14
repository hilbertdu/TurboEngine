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
	EngineCore::Instance().GetTaskScheduler()->PushTask(m_Task);
	return 0;
}

// Cancel
//------------------------------------------------------------------------------
/*virtual*/ uint32 ICommand::Cancel()
{
	return 0;
}

// Execute
//------------------------------------------------------------------------------
/*virtual*/ uint32 ExeCommandTask::Run()
{
	bool spawnOK = m_Process.Spawn(m_Executable.Get(),
		m_Arguments.IsEmpty() ? nullptr : m_Arguments.Get(), 
		m_WorkingDir.IsEmpty() ? nullptr : m_WorkingDir.Get(),
		m_Environment.IsEmpty() ? nullptr : m_Environment.Get());

	if (!spawnOK)
	{
		LERROR("Command", "Failed to spwan process '%s'\n", m_Executable.Get());
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