// TypeStruct.h
// Meta type struct
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_TYPESTRUCT_H
#define FOUNDATION_REFLECTION_TYPESTRUCT_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/ReflectionMacros.h"
#include "Foundation/Reflection/Members/Field.h"
#include "Foundation/Reflection/Members/Method.h"


namespace TReflection
{
	class MetaStruct;

	class IStruct : public IType 
	{
	public:
		using MetaType = MetaStruct;
		static constexpr MetaFlag s_MetaFlag = E_TYPE_STRUCT;
	};
	
	class MetaStruct : public IMetaType, public FieldCollection, public MethodCollection
	{
	public:
		MetaStruct() { m_Flag = IStruct::s_MetaFlag; }

		template<class FieldT>
		void AddField(const FieldT * member, const char * name);		// TODO: parameter attribute

		template<class C, class TReturn, class... TArgs>
		void AddMethod(TReturn(C::* member)(TArgs...), const char * name);

		template<class T>
		bool GetProperty(const void * object, const char * name, T & prop) const;

		template<class T>
		bool SetProperty(void * object, const char * name, const T & prop) const;

		template<class T>
		bool GetMethod(const char * name, T & method) const;

		inline uint32 GetFieldCount();

	public:
		MetaStruct*			m_Super{ 0 };
		Delegate<IType*()>	m_Creator;
		Delegate<IType*()>	m_Creator2;
	};

	template<class FieldT>
	void MetaStruct::AddField(const FieldT * member, const char * name)
	{
		using FieldType = std::remove_pointer<FieldT>::type;
		Array<Field>::Iter field = m_Fields.EmplaceAppend();
		field->m_Name = name;
		field->m_Index = (uint32)(m_Fields.GetSize() - 1);
		field->m_Size = sizeof(FieldT);
		field->m_Offset = (uint32)(UINTPTR)member;
		field->m_IsPointer = std::is_pointer<FieldT>::value;
		field->m_MetaType = MetaTypeDB::Instance().ObtainMetaType<FieldType>();
		ASSERT(field->m_MetaType);
	}

	template<class C, class TReturn, class... TArgs>
	void MetaStruct::AddMethod(TReturn(C::* member)(TArgs...), const char * name)
	{
		Array<Method>::Iter method = m_Methods.EmplaceAppend();
		method->m_Name = name;
		method->m_Index = (uint32)(m_Methods.GetSize() - 1);
		method->m_Delegate = AnyDelegate((C*)0, member);
	}

	template<class T>
	bool MetaStruct::GetProperty(const void * object, const char * name, T & prop) const
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

	template<class T>
	bool MetaStruct::GetMethod(const char * name, T & method) const
	{
		bool result = MethodCollection::GetMethod<T>(name, method);
		if (!result && m_Super)
		{
			result = m_Super->GetMethod<T>(name, method);
		}
		return result;
	}

	uint32 MetaStruct::GetFieldCount()
	{
		uint32 superSize = m_Super ? m_Super->GetFieldCount() : 0;
		return superSize + (uint32)m_Fields.GetSize();
	}
	REFLECTION_DECLARE_METAOBJECT(IStruct, IStruct::MetaType)
}


#endif // FOUNDATION_REFLECTION_TYPESTRUCT_H
