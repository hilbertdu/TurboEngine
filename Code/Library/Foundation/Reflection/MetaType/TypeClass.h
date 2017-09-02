// TypeClass.h
//------------------------------------------------------------------------------
// Meta class type 
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_TYPECLASS_H
#define FOUNDATION_REFLECTION_TYPECLASS_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/TypeStruct.h"
#include "Foundation/Pattern/Event.h"


namespace TReflection
{
	class MetaClass;

	class IClass : public IStruct
	{
	public:
		using MetaType = MetaClass;
	};

	class MetaClass : public MetaStruct
	{
	public:
		MetaClass() : MetaStruct() { SetFlag(E_TYPE_CLASS); }
	};
}


#endif // FOUNDATION_REFLECTION_TYPECLASS_H