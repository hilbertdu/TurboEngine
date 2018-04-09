// SerializerText.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_SERIALIZERPROPERTY_H
#define FOUNDATION_REFLECTION_SERIALIZERPROPERTY_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/Type.h"
#include "Foundation/Reflection/Serialization/Serializer.h"
#include "Foundation/Math/Math.h"

namespace TReflection
{
	template<class T, uint8 STYPE>
	struct PropertySerializer;
}

#include "Foundation/Reflection/Serialization/SerializerPropertyText.h"
//#include "Foundation/Reflection/Serialization/SerializerPropertyBinary.h"

//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_SERIALIZERPROPERTY_H