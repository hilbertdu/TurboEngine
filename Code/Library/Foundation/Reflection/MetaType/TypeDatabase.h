// TypeDatabase.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_TYPEDATABASE_H
#define FOUNDATION_REFLECTION_TYPEDATABASE_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/TypeDecl.h"
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
		template<typename T>
		IMetaType * GetMetaType(const T * t);
		IMetaType * GetMetaType(const Name & name);

	private:
		MetaTypeMap m_MetaTypes;
	};

	/*inline*/ void MetaTypeDB::RegisterAll()
	{
		RegisterAllMetaType(m_MetaTypes);
	}

	/*inline*/ void MetaTypeDB::Register(IMetaType * metaType)
	{
		m_MetaTypes[metaType->m_Name.m_Hash] = metaType;
	}

	template<typename T>
	IMetaType * MetaTypeDB::GetMetaType()
	{
		return GetMetaType(MetaDeduce<T>().Name);
	}

	template<typename T>
	IMetaType * MetaTypeDB::GetMetaType(const T * t)
	{
		// TODO: Get instance t meta type
		return nullptr;
	}

	IMetaType * MetaTypeDB::GetMetaType(const Name & name)
	{
		MetaTypeMap::Iterator iter = m_MetaTypes.Find(name.m_Hash);
		return iter != m_MetaTypes.End() ? (*iter).Second() : nullptr;
	}
}

#endif // FOUNDATION_REFLECTION_TYPEDATABASE_H
