// TypeStruct.h
// Meta type struct
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_TYPESTRUCT_H
#define FOUNDATION_REFLECTION_TYPESTRUCT_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/TypeDeclMacro.h"
#include "Foundation/Reflection/Members/Field.h"
#include "Foundation/Reflection/Members/Method.h"
#include "Foundation/Container/Array.h"


namespace TReflection
{
	class MetaStruct;

	class IStruct : public IType 
	{
	public:
		using MetaType = MetaStruct;
	};

	class MetaStruct : public IMetaType, public FieldCollection, public MethodCollection
	{
	public:
		MetaStruct() { SetFlag(E_TYPE_STRUCT); }

		template<class FieldT>
		void AddField(const FieldT * member, const char * name);		// TODO: parameter attribute

		template<class C, class TReturn, class... TArgs>
		void AddMethod(TReturn(C::* member)(TArgs...), const char * name);

		template<class T>
		bool GetProperty(void * object, const char * name, T & prop) const;

		template<class T>
		bool SetProperty(void * object, const char * name, const T & prop) const;

		template<class T>
		bool GetMethod(const char * name, T & method) const;

	private:
		template<class FieldT>
		void _AddField(Field& field, const std::false_type& UNUSED(isContainer));

		template<class FieldT>
		void _AddField(Field& field, const std::true_type& UNUSED(isContainer));

	public:
		MetaStruct*			m_Super{ 0 };
		Delegate<IType*()>	m_Creator;
	};

	template<class FieldT>
	void MetaStruct::_AddField(Field& field, const std::true_type&)
	{
		using FieldType = std::remove_pointer<FieldT>::type;
		using KeyType = MetaType<FieldType>::MetaKeyType;
		using ValueType = MetaType<FieldType>::MetaValueType;

		const Name& name = MetaType<FieldType>().m_Name;
		IMetaContainer* metaType = (IMetaContainer*)MetaTypeDB::Instance().GetMetaType(name);
		if (!metaType)
		{
			metaType = TNEW(MetaType<FieldType>);
			metaType->m_Name = name;
			metaType->m_MetaTypeKey = MetaTypeDB::Instance().GetMetaType<KeyType>();
			metaType->m_MetaTypeValue = MetaTypeDB::Instance().GetMetaType<ValueType>();
			MetaTypeDB::Instance().Register(metaType);
		}
		field.m_MetaType = metaType;
	}

	template<class FieldT>
	void MetaStruct::_AddField(Field& field, const std::false_type&)
	{
		using FieldType = std::remove_pointer<FieldT>::type;
		field.m_MetaType = MetaTypeDB::Instance().GetMetaType<FieldType>();
	}

	template<class FieldT>
	void MetaStruct::AddField(const FieldT * member, const char * name)
	{
		using FieldType = std::remove_pointer<FieldT>::type;
		Array<Field>::Iter field = m_Fields.EmplaceAppend();
		field->m_Name = name;
		field->m_Index = m_Fields.GetSize() - 1;
		field->m_Size = sizeof(FieldT);
		field->m_Offset = (UINTPTR)member;
		field->m_IsPointer = std::is_pointer<FieldT>::value;
		_AddField<FieldT>(*field, MetaType<FieldType>::IsContainerType());
		ASSERT(field->m_MetaType);
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
}


#endif // FOUNDATION_REFLECTION_TYPESTRUCT_H
