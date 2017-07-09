// TypeEnum.h
// Meta type enum
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_TYPEENUM_H
#define FOUNDATION_REFLECTION_TYPEENUM_H


#include "Foundation/Reflection/MetaType/Type.h"
#include "Foundation/Container/Array.h"

namespace TReflection
{
	class MetaTypeEnum : public IMetaType
	{
	public:
		typedef enum Innertype;

		template<class FieldT>
		void AddElement(FieldT * value, const AString & name);

	private:
		int32 m_Size{ sizeof(Innertype) };
	};
}


#endif // FOUNDATION_REFLECTION_TYPEENUM_H
