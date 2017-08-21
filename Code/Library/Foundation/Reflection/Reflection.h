// Reflection.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_H
#define FOUNDATION_REFLECTION_H

// Includes
//------------------------------------------------------------------------------

namespace TReflection
{
	class IStruct;
	class IClass;
	class IObject;

	void Initialization();
	IStruct* CreateStruct(const char * name);
	IClass*  CreateClass(const char * name);
	IObject* CreateObject(const char * name);
};

#include "Foundation/Reflection/Reflection.inl"

//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_H