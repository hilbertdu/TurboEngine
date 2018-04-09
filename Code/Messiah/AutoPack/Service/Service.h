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

	inline const AString & GetName() const { return m_Name; }
	inline const ICommand* GetCommand(const AStringView & name) const;	
	inline const Array<ICommand*> & GetCommands() const { return m_Commands; }

protected:
	AString m_Name;
	Array<ICommand*> m_Commands;

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