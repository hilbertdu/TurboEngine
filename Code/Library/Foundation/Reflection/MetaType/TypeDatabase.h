// TypeDatabase.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_TYPEDATABASE_H
#define FOUNDATION_REFLECTION_TYPEDATABASE_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/TypeDecl.h"
#include "Foundation/Reflection/Serialization/Serializer.h"
#include "Foundation/Container/HashMap.h"
#include "Foundation/Pattern/Singleton.h"

namespace TReflection
{
	class MetaTypeDB : public Singleton<MetaTypeDB>
	{
	public:
		using MetaTypeMap = HashMap<int32, IMetaType*>;

		inline void RegisterAll();
		inline void Register(IMetaType * metaType);

		template<typename T>
		IMetaType * GetMetaType();
		IMetaType * GetMetaType(const Name & name);
		IMetaType * GetMetaType(const StackName & name);
		IMetaType * GetMetaType(int32 hash);

		template<typename T>
		IMetaType* CreateMetaType();

		template<typename T>
		IMetaType* ObtainMetaType();

		void Clear();
		void Shrink();

	private:
		MetaTypeMap m_MetaTypes;
	};

	/*inline*/ void MetaTypeDB::RegisterAll()
	{
		RegisterAllMetaType(m_MetaTypes);
		RegisterAllSerializer(m_MetaTypes);
	}

	/*inline*/ void MetaTypeDB::Register(IMetaType * metaType)
	{
		m_MetaTypes[metaType->m_Name.m_Hash] = metaType;
	}

	void MetaTypeDB::Clear()
	{
		for (MetaTypeMap::Iter iter = m_MetaTypes.Begin(); iter != m_MetaTypes.End(); ++iter)
		{
			LOUTPUT("Clear metatype: %s\n", (*iter).Second()->m_Name.m_Name.Get());
			TDELETE_SAFE((*iter).Second());
		}
		m_MetaTypes.Clear();
	}

	void MetaTypeDB::Shrink()
	{
		m_MetaTypes.Shrink();
	}

	template<typename T>
	IMetaType * MetaTypeDB::GetMetaType()
	{
		return GetMetaType(MetaType<T>::StaticName());;
	}

	IMetaType * MetaTypeDB::GetMetaType(const Name & name)
	{
		MetaTypeMap::Iter iter = m_MetaTypes.Find(name.m_Hash);
		return iter != m_MetaTypes.End() ? (*iter).Second() : nullptr;
	}

	IMetaType * MetaTypeDB::GetMetaType(const StackName & name)
	{
		MetaTypeMap::Iter iter = m_MetaTypes.Find(name.m_Hash);
		return iter != m_MetaTypes.End() ? (*iter).Second() : nullptr;
	}

	IMetaType * MetaTypeDB::GetMetaType(int32 hash)
	{
		MetaTypeMap::Iter iter = m_MetaTypes.Find(hash);
		return iter != m_MetaTypes.End() ? (*iter).Second() : nullptr;
	}

	template<typename T>
	IMetaType* MetaTypeDB::CreateMetaType()
	{
		ASSERT(GetMetaType<T>() == nullptr);
		IMetaType* metaType = TNEW(MetaType<T>);
		Register(metaType);
		return metaType;
	}

	template<typename T>
	IMetaType* MetaTypeDB::ObtainMetaType()
	{
		IMetaType* metaType = GetMetaType<T>();
		return metaType || MetaType<T>::IsVoid::value ? metaType : CreateMetaType<T>();
	}
}

#endif // FOUNDATION_REFLECTION_TYPEDATABASE_H
