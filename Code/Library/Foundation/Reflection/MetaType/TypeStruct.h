// TypeStruct.h
// Meta type struct
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_TYPESTRUCT_H
#define FOUNDATION_REFLECTION_TYPESTRUCT_H


#include "Foundation/Reflection/MetaType/Type.h"
#include "Foundation/Reflection/Members/Field.h"
#include "Foundation/Reflection/Members/Method.h"
#include "Foundation/Container/Array.h"


namespace TReflection
{
	class IStruct : public IType {};

	class MetaStruct : public IMetaType, public FieldCollection, public MethodCollection
	{
	public:
		virtual bool IsStruct() { return true; }

		template<class FieldT>
		void AddField(const FieldT * member, const char * name);		// TODO: parameter attribute

		template<class C, class TReturn, class... TArgs>
		void AddMethod(TReturn(C::* member)(TArgs...), const char * name);

		template<class T>
		bool GetProperty(void * object, const char * name, T & prop) const;

		template<class T>
		bool SetProperty(void * object, const char * name, const T & prop) const;

	public:
		MetaStruct * m_Super;
	};

	template<class FieldT>
	void MetaStruct::AddField(const FieldT * member, const char * name)
	{
		Array<Field>::Iter field = m_Fields.EmplaceAppend();
		field->m_Name = name;
		field->m_Index = m_Fields.GetSize() - 1;
		field->m_Size = sizeof(FieldT);
		field->m_Offset = (UINTPTR)member;
		field->m_MetaType = MetaTypeDB::Instance().GetMetaType<FieldT>();
	}

	template<class C, class TReturn, class... TArgs>
	void MetaStruct::AddMethod(TReturn(C::* member)(TArgs...), const char * name)
	{
		Array<Method>::Iter method = m_Methods.EmplaceAppend();
		method->m_Name = name;
		method->m_Index = m_Methods.GetSize() - 1;
		method->m_Delegate = AnyDelegate((C*)0, member);
	}

	template<class T>
	bool MetaStruct::GetProperty(void * object, const char * name, T & prop) const
	{
		bool result = FieldCollection::GetProperty<T>(object, name, prop);
		if (!result && m_Super)
		{
			result = m_Super->GetProperty<T>(object, name, prop);
		}
		return result;
	}

	template<class T>
	bool MetaStruct::SetProperty(void * object, const char * name, const T & prop) const
	{
		bool result = FieldCollection::SetProperty<T>(object, name, prop);
		if (!result && m_Super)
		{
			result = m_Super->SetProperty<T>(object, name, prop);
		}
		return result;
	}

	// TODOs
	// 1. how to add enumeration info to meta database
	// 2. where does enumeration meta type store (store in self class as a static member)
	// 3. add GetMetaTypeS and GetMetaTypeV(if has) interface (to avoid each class has a virtual table)
#define _TREFLECTION_DECLARE(OBJECT, METATYPE) \
	public:									\
		using ObjectType = OBJECT;			\
		static METATYPE * GetMetaTypeS();	\
		static void RegisterMetaType();		\
		static void RegisterProperty();		\
		static void BindReflectionInfo();	\
	private:								\
		static METATYPE * s_MetaInfo;

#define _TREFLECTION_BEGIN(OBJECT, SUPER, METATYPE) \
	/*static*/ void OBJECT::RegisterMetaType()																\
	{																										\
		s_MetaInfo = TNEW(METATYPE);																		\
		s_MetaInfo->m_Name = TXT(#OBJECT);																	\
		s_MetaInfo->m_Super = static_cast<MetaStruct*>(MetaTypeDB::Instance().GetMetaType(TXT(#SUPER)));	\
		MetaTypeDB::Instance().Register(s_MetaInfo);														\
	}																										\
	_TREFLECT_STRUCT_COMMON(OBJECT, METATYPE)

#define _TREFLECT_STRUCT_COMMON(OBJECT, METATYPE) \
	/*static*/ METATYPE * OBJECT::GetMetaTypeS()											\
	{																						\
		return static_cast<METATYPE*>(MetaTypeDB::Instance().GetMetaType(TXT(#OBJECT)));	\
	}																						\
	/*static*/ void OBJECT::RegisterProperty()												\
	{

#define TREFLECT_FIELD(FIELD, FNAME)	\
		GetMetaTypeS()->AddField(&((ObjectType*)0)->FIELD, TXT(#FNAME));	\

#define TREFLECT_METHOD(METHOD, FNAME)	\
		GetMetaTypeS()->AddMethod(&ObjectType::METHOD, TXT(#FNAME));	\

#define _TREFLECTION_END(OBJECT, METATYPE)	\
	}												\
	/*static*/ void OBJECT::BindReflectionInfo()	\
	{												\
		RegisterMetaType();							\
		RegisterProperty();							\
	}

#define TREFLECTION_STRUCT_DECLARE(STRUCT)		_TREFLECTION_DECLARE(STRUCT, MetaStruct)
#define TREFLECT_STRUCT_BEGIN(STRUCT, SUPER)	_TREFLECTION_BEGIN(STRUCT, SUPER, MetaStruct)
#define TREFLECT_STRUCT_END(STRUCT)				_TREFLECTION_END(STRUCT, MetaStruct)
}


#endif // FOUNDATION_REFLECTION_TYPESTRUCT_H
