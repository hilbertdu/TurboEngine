// TypeObject.h
//------------------------------------------------------------------------------
// Meta class type 
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_TYPEOBJECT_H
#define FOUNDATION_REFLECTION_TYPEOBJECT_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/TypeClass.h"
#include "Foundation/Reflection/Objects/Object.h"
#include "Foundation/Pattern/Event.h"


namespace TReflection
{
	class MetaObject : public MetaClass
	{
	public:
		virtual bool IsObject() { return true; }

		virtual IMetaType * Create() const { return (IMetaType*)(TNEW(IObject)); }
	};
}


#define TREFLECTION_CLASS_DECLARE(CLASS)	_TREFLECTION_DECLARE(CLASS, TReflection::MetaClass)
#define TREFLECT_CLASS_BEGIN(CLASS, SUPER)	_TREFLECTION_BEGIN(CLASS, SUPER, TReflection::MetaClass)
#define TREFLECT_CLASS_END(CLASS)			_TREFLECTION_END(CLASS, TReflection::MetaClass)


#endif // FOUNDATION_REFLECTION_TYPEOBJECT_H