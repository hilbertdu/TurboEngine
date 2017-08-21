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
#include "Foundation/Math/CRC32.h"
#include "Foundation/Reflection/Serialization/Serializer.h"

namespace TReflection
{
	struct Name
	{
	public:
		Name() {};
		Name(const AString & name)
			: m_Name(name)
			, m_Hash(CRC32::Calc(name))
		{}
		Name(const char * name)
			: m_Name(name)
			, m_Hash(CRC32::Calc(name, AString::StrLen(name)))
		{}		

		int32	m_Hash;
		AString	m_Name;
	};

	bool operator==(const Name& lhs, const char * rhs) { return lhs.m_Name == rhs; }

	struct Primitive
	{
	public:
		Name m_Name;
	};

	class IMetaType : public Primitive
	{
	public:
		IMetaType() = default;
		IMetaType(const IMetaType&) = delete;
		IMetaType(IMetaType&&) = delete;
		virtual ~IMetaType() { TDELETE_SAFE(m_Serializer); }

		virtual bool IsBaseType() { return false; }
		virtual bool IsContainer() { return false; }
		virtual bool IsStruct() { return false; }
		virtual bool IsClass() { return false; }
		virtual bool IsObject() { return false; }

		virtual IMetaType* Create() const { return nullptr; };

	protected:
		int32			m_Size;
		ISerializer *	m_Serializer;
	};

	class IType	{};
}

#endif // FOUNDATION_REFLECTION_TYPE_H