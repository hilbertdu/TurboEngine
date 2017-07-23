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
	public:
		virtual bool IsBaseType() { return false; }
		virtual bool IsContainer() { return false; }

		template<class FieldT>
		void AddElement(FieldT value, const char * name);		// TODO: parameter attribute

		// Property Get/Set
		bool GetProperty(const char * name, void * prop) const;

		// Iteration (Fields)
		const Array<Field>& GetFields() const;

	private:
		Array<Field> m_Fields;
	};

	template<class FieldT>
	void MetaEnum::AddElement(FieldT value, const char * name)
	{
		m_Fields.EmplaceAppend();
		m_Fields.Last()->m_Name = name;
		m_Fields.Last()->m_Index = value;
		m_Fields.Last()->m_Size = sizeof(FieldT);
		m_Fields.Last()->m_MetaType = this;
	}

	bool MetaEnum::GetProperty(const char * name, void * prop) const
	{
		return true;
	}

	// TODOs
	// 1. how to add enumeration info to meta database
	// 2. where does enumeration meta type store (store in self class as a static member)
	// 3. add GetMetaTypeS and GetMetaTypeV(if has) interface (to avoid each class has a virtual table)
#define TREFLECTION_ENUM_DECLARE(ENUM)	\
	public:									\
		static MetaEnum * GetMetaTypeS();	\
		static void RegisterMetaType();		\
		static void RegisterProperty();		\
	private:								\
		static MetaEnum * s_MetaEnum;

#define TREFLECT_BEGIN(ENUM)	\
	/*static*/ void ENUM::RegisterMetaType()												\
	{																						\
		s_MetaEnum = TNEW(MetaEnum);														\
		s_MetaEnum->m_Name = TXT(#ENUM);													\
		MetaTypeDB::Instance().Register(s_MetaEnum);										\
	}																						\
	/*static*/ MetaEnum * ENUM::GetMetaTypeS()												\
	{																						\
		return static_cast<MetaEnum*>(MetaTypeDB::Instance().GetMetaType(TXT(#ENUM)));		\
	}																						\
	/*static*/ void ENUM::RegisterProperty()												\
	{

#define TREFLECT_FIELD(FIELD, FNAME)	\
		s_MetaEnum->AddElement(FIELD, TXT(FNAME));	\

#define TREFLECT_END(CLASS)	\
	}
	void BindReflectionInfo()
	{

	}
}


#endif // FOUNDATION_REFLECTION_TYPEENUM_H
