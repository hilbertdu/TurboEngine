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

	void InitSerializerLoad(Name name, SerializeType sType, SerializerLoad loadFunc)
	{
		MetaTypeDB::Instance().GetMetaType(name)->SetLoad(sType, loadFunc);
	}

	void InitSerializerSave(Name name, SerializeType sType, SerializerSave saveFunc)
	{
		MetaTypeDB::Instance().GetMetaType(name)->SetSave(sType, saveFunc);
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