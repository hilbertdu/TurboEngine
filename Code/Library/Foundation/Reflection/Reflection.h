// Reflection.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_H
#define FOUNDATION_REFLECTION_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/Type.h"

namespace TReflection
{
	class IStruct;
	class IClass;
	class IMetaType;

	static void Initialization();
	static void InitSerializerLoad(Name name, SerializeType sType, SerializerLoad loadFunc);
	static void InitSerializerSave(Name name, SerializeType sType, SerializerSave saveFunc);

	template<class T>
	static IMetaType* GetMetaType();

	static IStruct* CreateStruct(const char * name);
	static IClass*  CreateClass(const char * name);
};


#include "Foundation/Reflection/Reflection.inl"

//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_H