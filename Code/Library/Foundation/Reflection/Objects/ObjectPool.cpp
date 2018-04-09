// ObjectPool.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/ReflectionMacros.h"
#include "Foundation/Reflection/Objects/ObjectPool.h"
#include "Foundation/Reflection/Serialization/SerializerText.h"
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
	IMetaType * metaType = MetaTypeDB::Instance().GetMetaType(Name(name));
	ASSERT(metaType->IsObject());
	IObject* obj = (IObject*)((MetaClass*)metaType)->m_Creator.Invoke();
	obj->SetFlag(flag);
	return obj;
}

IObject* ObjectPool::CreateObject(const char * name, IMetaType * metaType)
{
	if (metaType == nullptr)
	{
		metaType = MetaTypeDB::Instance().GetMetaType(Name(name));
	}
	ASSERT(metaType->IsObject());
	return (IObject*)((MetaClass*)metaType)->m_Creator2.Invoke(metaType);
}

//------------------------------------------------------------------------------