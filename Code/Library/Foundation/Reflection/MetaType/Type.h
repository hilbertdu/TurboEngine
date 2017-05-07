// Type.h
//------------------------------------------------------------------------------
// Base types 
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_TYPE_H
#define FOUNDATION_REFLECTION_TYPE_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/String/String.h"
#include "Foundation/Pattern/Event.h"
#include "Foundation/Math/CRC32.h"
#include "Foundation/Reflection/Serialization/Serializer.h"

namespace TReflection
{
	struct Name
	{
	public:
		explicit Name() {};
		explicit Name(const AString & name)
			: m_Name(name)
			, m_Hash(CRC32::Calc(name))
		{}
		explicit Name(const char * name)
			: m_Name(name)
			, m_Hash(CRC32::Calc(name, AString::StrLen(name)))
		{}

		int32	m_Hash;
		AString	m_Name;
	};

	struct Primitive
	{
		Name m_Name;
	};

	class MetaType : public Primitive
	{
	public:
		int32			m_Size;
		ISerializer*	m_Serializer;
	};
}

#endif // FOUNDATION_REFLECTION_TYPE_H