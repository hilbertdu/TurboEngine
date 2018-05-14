// Parameter.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_SERVICE_COMMAND_H
#define MESSIAH_AUTOPACK_SERVICE_COMMAND_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/String/String.h"
#include "Foundation/String/StringView.h"
#include "Foundation/Process/Process.h"
#include "Foundation/Reflection/Objects/Object.h"
#include "Foundation/Task/Task.h"


// class IParameter
//------------------------------------------------------------------------------
class IParameter
{
public:
	IParameter() {}
	IParameter(AStringView name) : m_Name(name.Get()) {}
	virtual ~IParameter() {}

	const AString & GetName() const { return m_Name; }

private:
	AString m_Name;
};

// class ICommand
//------------------------------------------------------------------------------
class ICommand : public IObject
{
public:
	ICommand() : m_Task(nullptr) {}
	ICommand(AStringView name) : m_Name(name.Get()), m_Task(nullptr) {}
	virtual ~ICommand() {}

	ICommand(const ICommand &) = delete;

	inline const void SetDescription(const AStringView & desc) { m_Description = desc.Get(); }
	inline const void SetAuthor(const AStringView & author) { m_Author = author.Get(); }

	const AString &			GetName() const { return m_Name; }
	inline const AString &	GetDescription() const { return m_Description; }
	inline const AString &	GetAuthor() const { return m_Author; }
	inline const Task *		GetTask() const { return m_Task; }

	void SetParameterLen(uint32 length) { m_Parameters.SetSize(length); }
	void SetParameter(uint32 index, IParameter * parameter) { ASSERT(index < m_Parameters.GetSize()); m_Parameters[index] = parameter; }
	void SetParameter(AStringView name, IParameter * parameter) {}

	virtual uint32 Run();
	virtual uint32 Cancel();

	virtual bool TryLockOutput() { return false; }
	virtual void UnLockOutput() {}
	virtual const AStringView FetchOutput(uint32 index, uint32 len) const { return AStringView(); }	

protected:
	AString m_Name;
	AString m_Description;
	AString m_Author;
	Task *	m_Task;

	Array<IParameter*> m_Parameters;	

	TREFLECTION_DECLARE(ICommand, IObject);
};


// class ExeCommand
//------------------------------------------------------------------------------
class ExeCommandTask : public Task
{
public:
	explicit ExeCommandTask() {};
	virtual ~ExeCommandTask() { Cancel(); }

	virtual uint32 Run();
	virtual uint32 Cancel();

	inline void SetExecutable(const AStringView & exe) { m_Executable = exe.Get(); }
	inline void SetArguments(const AStringView & args) { m_Arguments = args.Get(); }
	inline void SetWorkingDir(const AStringView & dir) { m_WorkingDir = dir.Get(); }
	inline void SetEnvironment(const AStringView & env) { m_Environment = env.Get(); }

	virtual bool TryLockOutput() { m_OutputLock.TryLock(); }
	virtual void UnLockOutput() { m_OutputLock.UnLock(); }
	virtual const AStringView FetchOutput(uint32 index, uint32 len) const;

private:
	Process		m_Process;
	AString		m_Executable;
	AString		m_Arguments;
	AString		m_WorkingDir;
	AString		m_Environment;
	AString		m_Output;
	SpinLock	m_OutputLock;
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_SERVICE_COMMAND_H