// ObjectPool.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_OBJECTPOOL_H
#define FOUNDATION_REFLECTION_OBJECTPOOL_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Pattern/Singleton.h"
#include "Foundation/Objects/ObjectFlag.h"


class IObject;
namespace TReflection { class IMetaType; }

class ObjectPool : public Singleton<ObjectPool>
{
public:
	ObjectPool();
	~ObjectPool();

	IObject* CreateObject(const char * name, ObjectFlag flag = ObjectFlag::FLAG_OBJECT_NONE);
	IObject* CreateObject(const char * name, TReflection::IMetaType * metaType);
};


//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_OBJECTPOOL_H