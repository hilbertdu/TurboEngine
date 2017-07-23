// TypeContainer.h
// Type containers
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_TYPECONTAINER_H
#define FOUNDATION_REFLECTION_TYPECONTAINER_H


// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/Type.h"
#include "Foundation/Container/Array.h"
#include "Foundation/Container/Pair.h"


namespace TReflection
{
	class IMetaTypeContainer : public IMetaType
	{
	public:
		class IIterator
		{
		public:
			virtual void Begin() const = 0;
			virtual void End() const = 0;
			virtual void Next() = 0;
			virtual void Length() = 0;
			virtual void * GetKey() const = 0;
			virtual void * GetValue() const = 0;
		};

		template<class...>
		class Iterator;

		template<class T>
		class Iterator<Array<T, HeapAllocator>> : public IIterator
		{
		public:
			Iterator(const T * content)
				: m_Content(content) {}
			virtual void Begin() const {}
			virtual void End() const {}
			virtual void Next() {}
			virtual void Length() {}
			virtual void * GetKey() const { return nullptr; }
			virtual void * GetValue() const { return nullptr; }
		private:
			T * m_Content;
		};

		virtual ~IMetaTypeContainer() 
		{ 
			TDELETE m_MetaTypeKey;
			TDELETE m_MetaTypeValue;
			TDELETE m_Iterator; 
		}

		virtual bool IsBaseType() { return false; }
		virtual bool IsContainer() { return true; }

		template<class T>
		void Register(const T * t)
		{
			m_Iterator = TNEW(Iterator<T>(t));
			RegisterKeyType<GetMetaType<T>::MetaKeyType>(t);
			RegisterValueType<GetMetaType<T>::MetaValueType>(t);
		}

	private:
		template<class T>
		void RegisterKeyType(const T * t)
		{
			m_MetaTypeKey = MetaTypeDB::Instance().GetMetaType<T>();
			m_MetaTypeKey->Register<T>(t);
		}

		template<class T>
		void RegisterValueType(const T * t)
		{
			m_MetaTypeValue = MetaTypeDB::Instance().GetMetaType<T>();
			m_MetaTypeValue->Register<T>(t);
		}

		template<> void RegisterKeyType<void>(const void * t) {};
		template<> void RegisterValueType<void>(const void * t) {};

		IMetaType * m_MetaTypeKey;
		IMetaType * m_MetaTypeValue;
		IIterator * m_Iterator;
	};

	
}

#endif // FOUNDATION_REFLECTION_TYPECONTAINER_H
//------------------------------------------------------------------------------
