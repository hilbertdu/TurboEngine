// Field.h
// Meta type field
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_FIELD_H
#define FOUNDATION_REFLECTION_FIELD_H


// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/Type.h"
#include "Foundation/Reflection/MetaType/TypeEnum.h"

namespace TReflection
{
	class Field : public Primitive
	{
	public:
		Field();

	private:
		uint32		m_Index;
		uint32		m_Offset;
		uint32		m_Size;
		IMetaType*	m_MetaType;

		friend class MetaEnum;
	};
}

#endif // FOUNDATION_REFLECTION_FIELD_H
