// Reflection.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_H
#define FOUNDATION_REFLECTION_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/Type.h"
#include "Foundation/Reflection/Attributes/Attributes.h"
#include "Foundation/Reflection/ReflectionMacros.h"


namespace TReflection
{
	class IStruct;
	class IClass;
	class IMetaType;
	class MetaStruct;
	class MetaClass;
	class MetaTypeDB;
	class ISerializer;

	void Initialization();
	void InitSerializerLoad(Name name, SerializeType sType, SerializerLoad loadFunc);
	void InitSerializerSave(Name name, SerializeType sType, SerializerSave saveFunc);

	void Finalization();

	template<class T>
	IMetaType* GetMetaType();

	IMetaType* GetMetaType(Name name);

	IStruct* CreateStruct(const char * name);
	IClass*  CreateClass(const char * name);
};


//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_H