// Command.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Command.h"
#include "../Engine/EngineCore.h"
#include "../Event/EventDecl.h"
#include "Foundation/Process/Thread.h"
#include "Foundation/Process/Process.h"


void ICommand::AddOutput(const char * output)
{
	m_Output += output;
}

AStringView ICommand::FetchOutput(uint32 index, uint32 len) const
{
	ASSERT(index <= m_Output.GetLength());
	uint32 realLen = m_Output.GetLength() >= index + len ? len : m_Output.GetLength() - index;
	return AStringView(m_Output.Get() + index, realLen);
}


// Execute
//------------------------------------------------------------------------------
/*virtual*/ uint32 ExeCommand::Run()
{
	bool spawnOK = m_Process.Spawn(m_Parameters[0]->GetName().Get(), nullptr, nullptr, nullptr);

	if (!spawnOK)
	{
		LERROR("Command", "Failed to spwan process '%s'\n", m_Parameters[0]->GetName().Get());
		return false;
	}

	size_t bufferSize = MEGABYTE;
	size_t readSize = -1;
	char *output = TNEW(char[bufferSize]());
	for (; m_Process.IsRunning() || readSize != 0;)
	{
		readSize = m_Process.ReadStdOut(output, bufferSize - 1);
		output[readSize] = '\000';
		LOUTPUT(output);
		GetLock().Lock();
		AddOutput(output);
		GetLock().UnLock();
		Thread::Sleep(15);
	}
	TDELETE_SAFE(output);

	ASSERT(!m_Process.IsRunning());
	return m_Process.WaitForExit();
}

//------------------------------------------------------------------------------