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
	enum MetaFlag : uint16
	{
		E_NONE				= 0x00,
		E_TYPE_BASE			= 0x01,
		E_TYPE_STRUCT		= 0x02,
		E_TYPE_CLASS		= 0x04,
		E_TYPE_OBJECT		= 0x08,
		E_TYPE_CONTAINER	= 0x10
	};

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

		Name& Extend(const Name& other)
		{
			return operator+=(other);
		}

		Name& operator+=(const Name& other)
		{
			m_Name += other.m_Name;
			m_Hash = CRC32::Calc(m_Name.Get(), m_Name.GetLength());
			return *this;
		}

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
		virtual ~IMetaType() {}

		bool IsBaseType() const		{ return (m_Flag & E_TYPE_BASE) != 0; }
		bool IsContainer() const	{ return (m_Flag & E_TYPE_CONTAINER) != 0; }
		bool IsStruct() const		{ return (m_Flag & E_TYPE_STRUCT) != 0; }
		bool IsClass() const		{ return (m_Flag & E_TYPE_CLASS) != 0; }
		bool IsObject() const		{ return (m_Flag & E_TYPE_OBJECT) != 0; }

		void SetFlag(MetaFlag flag) { m_Flag = (MetaFlag)(m_Flag | flag); }		
		bool HasFlag(MetaFlag flag) const { return (m_Flag & flag) != 0; }
		MetaFlag GetFlag() const { return m_Flag; }

		void SetLoad(SerializeType sType, SerializerLoad loadFunc) { m_Serializer.m_LoadFunc[sType] = loadFunc; }
		void SetSave(SerializeType sType, SerializerSave saveFunc) { m_Serializer.m_SaveFunc[sType] = saveFunc; }

		SerializerLoad GetLoad(SerializeType sType) const { return m_Serializer.m_LoadFunc[sType]; }
		SerializerSave GetSave(SerializeType sType) const { return m_Serializer.m_SaveFunc[sType]; }

		void* Create() const { return nullptr; }

	protected:
		MetaFlag			m_Flag{ E_NONE };
		uint32				m_Size{ 0 };
		Serializer			m_Serializer;
	};

	class IType	{};
}

#endif // FOUNDATION_REFLECTION_TYPE_H