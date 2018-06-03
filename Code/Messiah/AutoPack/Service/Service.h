// Service.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_SERVICE_H
#define MESSIAH_AUTOPACK_SERVICE_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/String/String.h"
#include "Foundation/String/StringView.h"
#include "Foundation/Container/Array.h"
#include "Foundation/Container/SmartPtr.h"
#include "Foundation/Reflection/Objects/Object.h"


// Forward declaration
//------------------------------------------------------------------------------
class ICommand;
class WorkerThread;


// Class IService
//------------------------------------------------------------------------------
class IService : public IObject
{
public:
	IService();
	IService(const AStringView & name);
	virtual ~IService();	

	virtual void Initialize() {}
	virtual void Finalize() {}
	virtual void RunCommand(const AStringView & command);

	inline const void SetDescription(const AStringView & desc) { m_Description = desc.Get(); }
	inline const void SetAuthor(const AStringView & author) { m_Author = author.Get(); }

	inline const AString & GetName() const { return m_Name; }
	inline const AString & GetDescription() const { return m_Description; }
	inline const AString & GetAuthor() const { return m_Author; }

	inline ICommand*				GetCommand(const AStringView & name);
	inline const ICommand*			GetCommand(const AStringView & name) const;
	inline const Array<ICommand*> & GetCommands() const { return m_Commands; }
	inline const ICommand*			GetLastRunCommand() const { return m_LastRunCommand; }

	virtual ICommand * CreateCommand(const AStringView & name);

protected:
	AString m_Name;
	AString m_Description;
	AString m_Author;
	Array<ICommand*> m_Commands;

	ICommand* m_LastRunCommand;

	TREFLECTION_DECLARE(IService, IObject)
};


// Class IBatchService
//------------------------------------------------------------------------------
class IBatchService : public IObject
{
public:
	IBatchService() {}
	virtual ~IBatchService() {}

private:
	Array<IService*> m_Services;

	TREFLECTION_DECLARE(IBatchService, IObject)
};

//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_SERVICE_H