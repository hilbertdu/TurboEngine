// TypeDatabase.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_TYPEDATABASE_H
#define FOUNDATION_REFLECTION_TYPEDATABASE_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/Type.h"
#include "Foundation/Container/HashMap.h"

namespace TReflection
{
	class MetaTypeDB
	{
	public:
		using MetaTypeMap = HashMap<int32, MetaType*>;

		void RegisterAll();

		template<typename T>
		const MetaType * RegisterType(const Name & name);
		const MetaType * GetMetaType(const Name & name);

	private:
		HashMap<int32, MetaType*> m_MetaTypes;
	};


	template<typename T>
	const MetaType * MetaTypeDB::RegisterType(const Name & name)
	{
		MetaType * metatype = new MetaType();
		metatype->m_Name = name;
		metatype->m_Size = sizeof(T);
		m_MetaTypes[name.m_Hash] = metatype;
	}

	const MetaType * MetaTypeDB::GetMetaType(const Name & name)
	{
		MetaTypeMap::ConstIterator iter = m_MetaTypes.Find(name.m_Hash);
		return iter != m_MetaTypes.End() ? iter->Second() : nullptr;
	}
}

#endif // FOUNDATION_REFLECTION_TYPEDATABASE_H
