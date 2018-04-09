// Service.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Command.h"
#include "Service.h"
#include "../Engine/EngineCore.h"


// Constructor
//------------------------------------------------------------------------------
IService::IService()
{}

// Constructor
//------------------------------------------------------------------------------
IService::IService(const AStringView & name) 
	: m_Name(name.Get())
{}

// Destructor
//------------------------------------------------------------------------------
/*virtual*/ IService::~IService()
{
	for (auto item : m_Commands) { TDELETE_SAFE(item); };
}

// GetCommand
//------------------------------------------------------------------------------
const ICommand * IService::GetCommand(const AStringView & name) const
{
	ICommand ** result = m_Commands.FindIf([name](const ICommand * cmd) { return cmd->GetName() == name.Get(); });
	return result ? *result : nullptr;
}

// RunCommand
//------------------------------------------------------------------------------
/*virtual*/ void IService::RunCommand(const AStringView & command)
{
	ICommand ** cmd = m_Commands.FindIf([command](const ICommand * cmd) { return cmd->GetName() == command.Get(); });
	ASSERT(cmd);

	if (cmd)
	{
		EngineCore::Instance().GetTaskScheduler()->PushTask(*cmd);
	}
}

//------------------------------------------------------------------------------