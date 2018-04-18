// ServiceManager.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "ServiceManager.h"
#include "../Service/Service.h"
#include "../Service/ResCooker/Chef.h"
#include "../Service/Test/Test.h"
#include "../Service/ResCooker/MixmapMerge.h"
#include "../Service/ResCooker/UIPacker.h"
#include "../Service/Patch/PatchGenerator.h"


// Constructor
//------------------------------------------------------------------------------
ServiceManager::ServiceManager()
{
	for (auto & service : m_Services)
	{		
		service->Initialize();		
	}
}

ServiceManager::~ServiceManager()
{
	for (auto & service : m_Services)
	{
		service->Finalize();
	}
}


IService * ServiceManager::GetService(const AStringView & service)
{
	IService ** result = m_Services.FindIf([service](const IService * s) { return s->GetName() == service.Get(); });
	return result ? *result : nullptr;
}

const IService * ServiceManager::GetService(const AStringView & service) const
{
	IService ** result = m_Services.FindIf([service](const IService * s) { return s->GetName() == service.Get(); });
	return result ? *result : nullptr;
}

void ServiceManager::AddService(const IService * service)
{

}

void ServiceManager::RemoveService(const AStringView & service)
{

}

//------------------------------------------------------------------------------