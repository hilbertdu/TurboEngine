// TypeClass.h
//------------------------------------------------------------------------------
// Meta class type 
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_TYPECLASS_H
#define FOUNDATION_REFLECTION_TYPECLASS_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/Type.h"
#include "Foundation/Reflection/MetaType/TypeStruct.h"
#include "Foundation/Pattern/Event.h"


namespace TReflection
{
	class IClass : public IStruct {};

	class MetaClass : public MetaStruct
	{
	public:
		virtual bool IsClass() { return true; }

	private:
		AnyDelegate m_Constructor;
		AnyDelegate m_Destructor;
	};

#define TREFLECTION_CLASS_DECLARE(CLASS)	_TREFLECTION_DECLARE(CLASS, MetaClass)
#define TREFLECT_CLASS_BEGIN(CLASS, SUPER)	_TREFLECTION_BEGIN(CLASS, SUPER, MetaClass)
#define TREFLECT_CLASS_END(CLASS)			_TREFLECTION_END(CLASS, MetaClass)
}

#endif // FOUNDATION_REFLECTION_TYPECLASS_H