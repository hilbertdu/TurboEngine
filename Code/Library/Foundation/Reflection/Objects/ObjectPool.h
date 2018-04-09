// ObjectPool.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_OBJECTPOOL_H
#define FOUNDATION_REFLECTION_OBJECTPOOL_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Pattern/Singleton.h"
#include "Foundation/Container/HashMap.h"
#include "Foundation/FileIO/MemRStream.h"
#include "Foundation/FileIO/MemWStream.h"
#include "Foundation/Reflection/Objects/Object.h"


class IObject;
namespace TReflection { class IMetaType; };

class ObjectPool : public Singleton<ObjectPool>
{
public:
	typedef HashMap<UINTPTR, IObject*> ObjectMap;

	ObjectPool();
	~ObjectPool();

	IObject* CreateObject(const char * name, ObjectFlag flag = ObjectFlag::FLAG_OBJECT_NONE);
	IObject* CreateObject(const char * name, TReflection::IMetaType * metaType);
};


//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_OBJECTPOOL_H