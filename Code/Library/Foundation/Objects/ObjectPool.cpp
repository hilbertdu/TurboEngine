// ObjectPool.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Objects/ObjectPool.h"
#include "Foundation/Objects/Object.h"
#include "Foundation/FileIO/MemWStream.h"
#include "Foundation/FileIO/MemRStream.h"

// Constructor
//------------------------------------------------------------------------------
ObjectPool::ObjectPool()
{
}

// Destructor
//------------------------------------------------------------------------------
ObjectPool::~ObjectPool()
{
}

// CreateObject
//------------------------------------------------------------------------------
IObject* ObjectPool::CreateObject(const char * name, ObjectFlag flag)
{
	TReflection::IMetaType * metaType = TReflection::GetMetaType(TReflection::Name(name));
	ASSERT(metaType->IsObject());
	IObject* obj = (IObject*)((TReflection::MetaClass*)metaType)->m_Creator.Invoke();
	obj->SetFlag(flag);
	return obj;
}

IObject* ObjectPool::CreateObject(const char * name, TReflection::IMetaType * metaType)
{
	if (metaType == nullptr)
	{
		metaType = TReflection::MetaTypeDB::Instance().GetMetaType(TReflection::Name(name));
	}
	ASSERT(metaType->IsObject());
	return (IObject*)((TReflection::MetaClass*)metaType)->m_Creator2.Invoke(metaType);
}

//------------------------------------------------------------------------------