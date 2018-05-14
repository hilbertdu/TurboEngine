// ServiceManager.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "ServiceManager.h"
#include "AutoPack/Service/Service.h"
#include "AutoPack/Service/ResCooker/Chef.h"
#include "AutoPack/Service/Test/Test.h"
#include "AutoPack/Service/ResCooker/MixmapMerge.h"
#include "AutoPack/Service/ResCooker/UIPacker.h"
#include "AutoPack/Service/Patch/PatchGenerator.h"
#include "Foundation/Reflection/Objects/ObjectPool.h"


// Constructor
//------------------------------------------------------------------------------
ServiceManager::ServiceManager()
{
	for (auto & service : m_Services)
	{		
		service->Initialize();		
	}
}

// Destructor
//------------------------------------------------------------------------------
ServiceManager::~ServiceManager()
{
	for (auto & service : m_Services)
	{
		service->Finalize();
	}
}

IService * ServiceManager::GetService(const AStringView & name)
{
	IService ** result = m_Services.FindIf([name](const IService * s) { return s->GetName() == name.Get(); });
	return result ? *result : nullptr;
}

const IService * ServiceManager::GetService(const AStringView & name) const
{
	IService ** result = m_Services.FindIf([name](const IService * s) { return s->GetName() == name.Get(); });
	return result ? *result : nullptr;
}

IService * ServiceManager::CreateService(const AStringView & name)
{
	IService * service = GetService(name);
	if (!service)
	{
		service = static_cast<IService*>(ObjectPool::Instance().CreateObject("IService"));
		service->SetProperty("Name", AString(name.Get()));
		m_Services.Append(service);
	}
	return service;
}

void ServiceManager::RemoveService(const AStringView & name)
{
}

//------------------------------------------------------------------------------