// ServiceManager.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_SERVICEMANAGER_H
#define MESSIAH_AUTOPACK_SERVICEMANAGER_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Container/Array.h"
#include "Foundation/String/StringView.h"


class IService;

class ServiceManager
{
public:
	ServiceManager();
	~ServiceManager();

	inline const Array<IService*> & GetServices() const { return m_Services; }

	IService *		 GetService(const AStringView & service);
	const IService * GetService(const AStringView & service) const;

private:
	Array<IService*> m_Services;
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_SERVICEMANAGER_H