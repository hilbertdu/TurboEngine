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


namespace TReflection
{
	class MetaClass;

	class IClass : public IStruct
	{
	public:
		using MetaType = MetaClass;
		static constexpr MetaFlag s_MetaFlag = E_TYPE_CLASS;
	};

	class MetaClass : public MetaStruct
	{
	public:
		MetaClass() : MetaStruct() { m_Flag = IClass::s_MetaFlag; }
	};
	REFLECTION_DECLARE_METAOBJECT(IClass, IClass::MetaType)
}


#endif // FOUNDATION_REFLECTION_TYPECLASS_H