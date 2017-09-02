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
		IMetaType * GetMetaType(int32 hash);

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
		return GetMetaType(MetaType<T>().m_Name);
	}

	IMetaType * MetaTypeDB::GetMetaType(const Name & name)
	{
		MetaTypeMap::Iter iter = m_MetaTypes.Find(name.m_Hash);
		return iter != m_MetaTypes.End() ? (*iter).Second() : nullptr;
	}

	IMetaType * MetaTypeDB::GetMetaType(int32 hash)
	{
		MetaTypeMap::Iter iter = m_MetaTypes.Find(hash);
		return iter != m_MetaTypes.End() ? (*iter).Second() : nullptr;
	}
}

#endif // FOUNDATION_REFLECTION_TYPEDATABASE_H
