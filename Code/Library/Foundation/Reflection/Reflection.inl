// Reflection.inl
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/TypeDatabase.h"
#include "Foundation/Reflection/MetaType/TypeStruct.h"
#include "Foundation/Reflection/MetaType/TypeClass.h"
#include "Foundation/Reflection/Objects/Object.h"


namespace TReflection
{
	// Initialization
	//------------------------------------------------------------------------------
	void Initialization()
	{
		MetaTypeDB::Initialize();
		MetaTypeDB::Instance().RegisterAll();
	}

	// CreateStruct
	//------------------------------------------------------------------------------
	IStruct* CreateStruct(const char * name)
	{
		IMetaType* metaType = MetaTypeDB::Instance().GetMetaType(Name(name));
		ASSERT(metaType->IsStruct());
		return (IStruct*)((MetaStruct*)metaType)->m_Creator.Invoke();
	}

	// CreateStruct
	//------------------------------------------------------------------------------
	IClass*  CreateClass(const char * name)
	{
		IMetaType* metaType = MetaTypeDB::Instance().GetMetaType(Name(name));
		ASSERT(metaType->IsClass());
		return (IClass*)((MetaClass*)metaType)->m_Creator.Invoke();
	}

	// CreateObject
	//------------------------------------------------------------------------------
	IObject* CreateObject(const char * name)
	{
		IMetaType* metaType = MetaTypeDB::Instance().GetMetaType(Name(name));
		ASSERT(metaType->IsClass());
		return (IObject*)((MetaClass*)metaType)->m_Creator.Invoke();
	}
}

//------------------------------------------------------------------------------