// Parameter.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_SERVICE_COMMAND_H
#define MESSIAH_AUTOPACK_SERVICE_COMMAND_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/String/String.h"
#include "Foundation/String/StringView.h"
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
class ICommand : public Task
{
public:
	ICommand(AStringView name) : m_Name(name.Get()) {}
	virtual ~ICommand() {}

	const AString & GetName() const { return m_Name; }
	void SetParameterLen(uint32 length) { m_Parameters.SetSize(length); }
	void SetParameter(uint32 index, IParameter * parameter) { ASSERT(index < m_Parameters.GetSize()); m_Parameters[index] = parameter; }
	void SetParameter(AStringView name, IParameter * parameter) {}

	void		AddOutput(const char * output);	
	AStringView FetchOutput(uint32 index, uint32 len) const;

	inline SpinLock & GetLock() { return m_OutputLock; }

protected:
	AString m_Name;
	Array<IParameter*> m_Parameters;

	AString  m_Output;
	SpinLock m_OutputLock;
};


// class ExeCommand
//------------------------------------------------------------------------------
class ExeCommand : public ICommand
{
public:
	ExeCommand(AStringView name) : ICommand(name) {}
	virtual ~ExeCommand() {}

	virtual uint32 Run();
};

// class PythonCommand
//------------------------------------------------------------------------------
class PythonCommand : public ExeCommand
{
public:
	PythonCommand(AStringView name) : ExeCommand(name) {}
	virtual ~PythonCommand() {}
};

//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_SERVICE_COMMAND_H