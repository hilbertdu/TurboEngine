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


// class ExeCommand
//------------------------------------------------------------------------------
class ExeCommandTask : public Task
{
public:
	explicit ExeCommandTask() {};
	ExeCommandTask(const AStringView & exe, const AStringView & args, const AStringView & dir, const AStringView & env);
	virtual ~ExeCommandTask() { Cancel(); }

	inline void SetExecutable(const AStringView & exe) { m_Executable = exe.Get(); }
	inline void SetArguments(const AStringView & args) { m_Arguments = args.Get(); }
	inline void SetWorkingDir(const AStringView & dir) { m_WorkingDir = dir.Get(); }
	inline void SetEnvironment(const AStringView & env) { m_Environment = env.Get(); }

	virtual bool TryLockOutput() { return m_OutputLock.TryLock(); }
	virtual void UnLockOutput() { m_OutputLock.UnLock(); }
	virtual const AStringView FetchOutput(uint32 index, uint32 len) const;

protected:
	virtual uint32 Excute();
	virtual uint32 Cancel();

private:
	Process		m_Process;
	AString		m_Executable;
	AString		m_Arguments;
	AString		m_WorkingDir;
	AString		m_Environment;
	AString		m_Output;
	SpinLock	m_OutputLock;
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

	inline void SetDescription(const AStringView & desc) { m_Description = desc.Get(); }
	inline void SetAuthor(const AStringView & author) { m_Author = author.Get(); }
	inline void SetExecutable(const AStringView & executable) { m_Executable = executable.Get(); }
	inline void SetParameters(const AStringView & parameters) { m_Parameters = parameters.Get(); }
	inline void SetWorkingDir(const AStringView & workdingDir) { m_WorkingDir = workdingDir.Get(); }
	inline void SetEnvironment(const AStringView & environment) { m_Environment = environment.Get(); }

	const AString &			GetName() const { return m_Name; }
	inline const AString &	GetDescription() const { return m_Description; }
	inline const AString &	GetAuthor() const { return m_Author; }
	inline Task::Status		GetStatus() const { return m_Task->GetStatus(); }

	//void SetParameterLen(uint32 length) { m_Parameters.SetSize(length); }
	//void SetParameter(uint32 index, IParameter * parameter) { ASSERT(index < m_Parameters.GetSize()); m_Parameters[index] = parameter; }
	//void SetParameter(AStringView name, IParameter * parameter) {}

	virtual uint32 Run();
	virtual uint32 Cancel();

	inline bool IsRunning() const { return m_Task.Get() != nullptr; }

	inline bool TryLockOutput() { return static_cast<ExeCommandTask*>(m_Task.Get())->TryLockOutput(); }
	inline void UnLockOutput() { static_cast<ExeCommandTask*>(m_Task.Get())->UnLockOutput(); }
	inline const AStringView FetchOutput(uint32 index, uint32 len) const { return static_cast<const ExeCommandTask*>(m_Task.Get())->FetchOutput(index, len); }

protected:
	AString m_Name;
	AString m_Description;
	AString m_Author;	

	//Array<IParameter*> m_Parameters;
	AString m_Executable;
	AString m_Parameters;
	AString m_WorkingDir;
	AString m_Environment;

	StrongPtr<Task> m_Task;

	TREFLECTION_DECLARE(ICommand, IObject);
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_SERVICE_COMMAND_H