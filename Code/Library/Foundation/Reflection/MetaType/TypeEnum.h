// TypeEnum.h
// Meta type enum
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_TYPEENUM_H
#define FOUNDATION_REFLECTION_TYPEENUM_H


#include "Foundation/Reflection/MetaType/Type.h"
#include "Foundation/Reflection/Members/Field.h"
#include "Foundation/Container/Array.h"


namespace TReflection
{
	class IEnumeration : public IType {};

	class MetaEnum : public IMetaType
	{
	};
}

#endif // FOUNDATION_REFLECTION_TYPEENUM_H
