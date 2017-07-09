// TypeDa.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_TYPEDATABASE_H
#define FOUNDATION_REFLECTION_TYPEDATABASE_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/TypeDecl.h"
#include "Foundation/Container/HashMap.h"

namespace TReflection
{
	class MetaTypeDB
	{
	public:
		using MetaTypeMap = HashMap<int32, IMetaType*>;

		inline void RegisterAll();

		template<typename T>
		const IMetaType * RegisterType(const Name & name);
		const IMetaType * GetMetaType(const Name & name);

	private:
		HashMap<int32, IMetaType*> m_MetaTypes;
	};


	inline void MetaTypeDB::RegisterAll()
	{
		RegisterAllMetaType(m_MetaTypes);
	}

	template<typename T>
	const IMetaType * MetaTypeDB::RegisterType(const Name & name)
	{
		IMetaType * metatype = new MetaType<T>();
		m_MetaTypes[name.m_Hash] = metatype;
	}

	const IMetaType * MetaTypeDB::GetMetaType(const Name & name)
	{
		MetaTypeMap::ConstIterator iter = m_MetaTypes.Find(name.m_Hash);
		return iter != m_MetaTypes.End() ? iter->Second() : nullptr;
	}
}

#endif // FOUNDATION_REFLECTION_TYPEDATABASE_H
