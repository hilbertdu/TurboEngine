// Field.h
// Meta type field
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_FIELD_H
#define FOUNDATION_REFLECTION_FIELD_H


// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/Type.h"
#include "Foundation/Reflection/Attributes/Attribute.h"
#include "Foundation/Env/Assert.h"
#include "Foundation/Container/Array.h"

namespace TReflection
{
	class IAttribute;

	class Field : public Primitive
	{
	public:
		Field() 
			: m_Index(0)
			, m_Offset(0)
			, m_Size(0)
			, m_IsPointer(false)
			, m_MetaType(0)
			, m_Attributes(E_ATTR_COUNT, nullptr) 
		{ 
			ASSERT(m_Attributes.GetSize() == E_ATTR_COUNT);
		}

		~Field()
		{
			ClearAttributes();
		}

		Field(const Field & other) = delete;

		Field(Field && other)			
			: Primitive(std::move(other.m_Name))
			, m_Index(other.m_Index)
			, m_Offset(other.m_Offset)
			, m_Size(other.m_Size)
			, m_IsPointer(other.m_IsPointer)
			, m_MetaType(other.m_MetaType)
			, m_Attributes(std::move(other.m_Attributes))
		{
			other.m_Attributes.Clear();
		}

		template<class T>
		inline void GetProperty(const void * object, const char * name, T & prop) const
		{
			prop = *(T *)((UINTPTR)object + m_Offset);
		}

		template<class T>
		inline void SetProperty(void * object, const char * name, const T & prop) const
		{
			*(T *)((UINTPTR)object + m_Offset) = prop;
		}

		template<class T>
		inline const T * GetAttribute() const
		{
			ASSERT(T::AttrType < E_ATTR_COUNT);
			return dynamic_cast<T*>(m_Attributes[T::AttrType]);
		}

		template<class T>
		inline T * GetAttribute()
		{
			ASSERT(T::AttrType < E_ATTR_COUNT);
			return dynamic_cast<T*>m_Attributes[T::AttrType];
		}

		template<class T1, class... TArgs>
		void AddAttribute(T1&& t1, TArgs&&... others)
		{
			m_Attributes[T::AttrType] = TNEW(T(std::forward<T>(t1)));
			AddAttribute(std::forward<TArgs>(others)...);
		}

		template<class T>
		void AddAttribute(T&& t)
		{
			m_Attributes[T::AttrType] = TNEW(T(std::forward<T>(t)));
		}

		inline void AddAttribute() {}

		void ClearAttributes()
		{
			for (Array<IAttribute*>::Iter iter = m_Attributes.Begin(); iter != m_Attributes.End(); ++iter)
			{
				TDELETE_SAFE(*iter);
			}
		}

	public:
		uint32		m_Index;
		uint32		m_Offset;
		uint32		m_Size;
		bool		m_IsPointer;
		IMetaType*	m_MetaType;

		Array<IAttribute*> m_Attributes;
	};

	class FieldCollection
	{
	public:
		template<class T>
		bool GetProperty(const void * object, const char * name, T & prop) const;

		template<class T>
		bool SetProperty(void * object, const char * name, const T & prop) const;

		inline const Array<Field> & GetFields() const { return m_Fields; }
		inline Field & TopField() { return m_Fields.TopItem(); }

	protected:
		Array<Field> m_Fields;
	};

	template<class T>
	bool FieldCollection::GetProperty(const void * object, const char * name, T & prop) const
	{
		Field * pField = m_Fields.FindIf([name](const Field& field) { return field.m_Name == name; });
		if (pField == nullptr)
		{
			return false;
		}
		pField->GetProperty<T>(object, name, prop);
		return true;
	}

	template<class T>
	bool FieldCollection::SetProperty(void * object, const char * name, const T & prop) const
	{
		Field * pField = m_Fields.FindIf([name](const Field& field) { return field.m_Name == name; });
		if (pField == nullptr)
		{
			return false;
		}
		pField->SetProperty<T>(object, name, prop);
		return true;
	}
}

#endif // FOUNDATION_REFLECTION_FIELD_H
