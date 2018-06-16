// TypeDatabase.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_TYPEDATABASE_H
#define FOUNDATION_REFLECTION_TYPEDATABASE_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Container/HashMap.h"
#include "Foundation/Pattern/Singleton.h"
#include "Foundation/Reflection/MetaType/TypeDecl.h"


namespace TReflection
{
	class MetaTypeDB : public Singleton<MetaTypeDB>
	{
	public:
		using MetaTypeMap = HashMap<int32, IMetaType*>;

		inline void RegisterAll()
		{
			RegisterAllMetaType(m_MetaTypes);
			RegisterAllSerializer(m_MetaTypes);
		}

		inline void Register(IMetaType * metaType)
		{
			ASSERT(m_MetaTypes.Find(metaType->m_Name.m_Hash) == m_MetaTypes.End());
			m_MetaTypes[metaType->m_Name.m_Hash] = metaType;
		}

		template<typename T>
		inline IMetaType * GetMetaType()
		{
			return GetMetaType(MetaType<T>::StaticName());;
		}

		inline IMetaType * GetMetaType(const Name & name)
		{
			MetaTypeMap::Iter iter = m_MetaTypes.Find(name.m_Hash);
			return iter != m_MetaTypes.End() ? (*iter).Second() : nullptr;
		}

		inline IMetaType * GetMetaType(const StackName & name)
		{
			MetaTypeMap::Iter iter = m_MetaTypes.Find(name.m_Hash);
			return iter != m_MetaTypes.End() ? (*iter).Second() : nullptr;
		}

		inline IMetaType * GetMetaType(int32 hash)
		{
			MetaTypeMap::Iter iter = m_MetaTypes.Find(hash);
			return iter != m_MetaTypes.End() ? (*iter).Second() : nullptr;
		}

		template<typename T>
		IMetaType* CreateMetaType()
		{
			ASSERT(GetMetaType<T>() == nullptr);
			MetaType<T> * metaType = TNEW(MetaType<T>);
			Register(metaType);
			return metaType;
		}

		template<typename T>
		IMetaType* ObtainMetaType()
		{
			IMetaType* metaType = GetMetaType<T>();
			return metaType || MetaType<T>::IsVoid::value ? metaType : CreateMetaType<T>();
		}

		void Clear()
		{
			for (MetaTypeMap::Iter iter = m_MetaTypes.Begin(); iter != m_MetaTypes.End(); ++iter)
			{
				LOUTPUT("Clear metatype: %s\n", (*iter).Second()->m_Name.m_Name.Get());
				TDELETE_SAFE((*iter).Second());
			}
			m_MetaTypes.Clear();
		}

		void Shrink()
		{
			m_MetaTypes.Shrink();
		}

	private:
		MetaTypeMap m_MetaTypes;
	};
}

#endif // FOUNDATION_REFLECTION_TYPEDATABASE_H
