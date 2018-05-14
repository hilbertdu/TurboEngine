// Service.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Command.h"
#include "Service.h"
#include "Foundation/Reflection/Objects/ObjectPool.h"
#include "AutoPack/Engine/EngineCore.h"


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
ICommand * IService::GetCommand(const AStringView & name)
{
	ICommand ** result = m_Commands.FindIf([name](const ICommand * cmd) { return cmd->GetName() == name.Get(); });
	return result ? *result : nullptr;
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
	(*cmd)->Run();
}

// CreateCommand
//------------------------------------------------------------------------------
/*virtual*/ ICommand * IService::CreateCommand(const AStringView & cls, const AStringView & name)
{
	ICommand * command = GetCommand(name);
	if (!command)
	{
		command = static_cast<ICommand*>(ObjectPool::Instance().CreateObject(cls.Get()));
		command->SetProperty("Name", AString(name.Get()));
		m_Commands.Append(command);
	}
	return command->GetName() == cls.Get() ? command : nullptr;
}

//------------------------------------------------------------------------------