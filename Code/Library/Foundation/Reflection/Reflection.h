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
	class IObject;

	void Initialization();
	void InitSerializerLoad(Name name, SerializeType sType, SerializerLoad loadFunc);
	void InitSerializerSave(Name name, SerializeType sType, SerializerSave saveFunc);

	IStruct* CreateStruct(const char * name);
	IClass*  CreateClass(const char * name);
	IObject* CreateObject(const char * name);
};


#include "Foundation/Reflection/Reflection.inl"

//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_H