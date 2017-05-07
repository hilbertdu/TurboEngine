// TypeEnum.h
// Meta type enum
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_FIELD_H
#define FOUNDATION_REFLECTION_FIELD_H


// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/Type.h"

namespace TReflection
{
	class Field : public MetaType
	{
	public:
		Field();

	private:
		uint32 m_Offset;
		uint32 m_Index;
	};
}

#endif // FOUNDATION_REFLECTION_FIELD_H
