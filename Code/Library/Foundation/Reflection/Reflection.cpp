// Reflection.inl
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/TypeDecl.h"
#include "Foundation/Reflection/MetaType/TypeClass.h"
#include "Foundation/Reflection/MetaType/TypeDatabase.h"
#include "Foundation/Reflection/Reflection.h"


extern void BindReflectionFoundation();


namespace TReflection
{
	// Initialization
	//------------------------------------------------------------------------------
	/*static*/ void Initialization()
	{
		MetaTypeDB::Initialize();
		MetaTypeDB::Instance().RegisterAll();
		BindReflectionFoundation();
	}

	/*static*/ void InitSerializerLoad(Name name, SerializeType sType, SerializerLoad loadFunc)
	{
		MetaTypeDB::Instance().GetMetaType(name)->SetLoad(sType, loadFunc);
	}

	/*static*/ void InitSerializerSave(Name name, SerializeType sType, SerializerSave saveFunc)
	{
		MetaTypeDB::Instance().GetMetaType(name)->SetSave(sType, saveFunc);
	}

	// Finalization
	//------------------------------------------------------------------------------
	/*static*/ void Finalization()
	{
		MetaTypeDB::Instance().Clear();
		MetaTypeDB::Finalize();
	}

	template<class T>
	/*static*/ IMetaType* GetMetaType()
	{
		return MetaTypeDB::Instance().GetMetaType<T>();
	}

	IMetaType* GetMetaType(Name name)
	{
		return MetaTypeDB::Instance().GetMetaType(name);
	}

	// CreateStruct
	//------------------------------------------------------------------------------
	/*static*/ IStruct* CreateStruct(const char * name)
	{
		IMetaType* metaType = MetaTypeDB::Instance().GetMetaType(Name(name));
		ASSERT(metaType->IsStruct());
		return (IStruct*)((MetaStruct*)metaType)->m_Creator.Invoke();
	}

	// CreateStruct
	//------------------------------------------------------------------------------
	/*static*/ IClass*  CreateClass(const char * name)
	{
		IMetaType* metaType = MetaTypeDB::Instance().GetMetaType(Name(name));
		ASSERT(metaType->IsClass());
		return (IClass*)((MetaClass*)metaType)->m_Creator.Invoke();
	}	
}

//------------------------------------------------------------------------------