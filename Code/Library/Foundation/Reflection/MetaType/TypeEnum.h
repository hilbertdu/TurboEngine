// TypeEnum.h
// Meta type enum
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_TYPEENUM_H
#define FOUNDATION_REFLECTION_TYPEENUM_H


#include "Foundation/Reflection/MetaType/Type.h"


class MetaTypeEnum : public MetaType
{
public:
	typedef enum Innertype;

	void AddElement(uint32 value, const AString & name);
	void AddElement()

private:
	int32 m_Size{ sizeof(Innertype) };
};


#endif // FOUNDATION_REFLECTION_TYPEENUM_H
