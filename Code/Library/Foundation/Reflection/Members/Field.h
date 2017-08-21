// Field.h
// Meta type field
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_FIELD_H
#define FOUNDATION_REFLECTION_FIELD_H


// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/Type.h"
#include "Foundation/Env/Assert.h"

namespace TReflection
{
	class Field : public Primitive
	{
	public:
		Field() : m_Index(0), m_Offset(0), m_Size(0), m_MetaType(0) {}

		template<class T>
		inline void GetProperty(void * object, const char * name, T & prop) const
		{
			prop = *(T *)((UINTPTR)object + m_Offset);
		}

		template<class T>
		inline void SetProperty(void * object, const char * name, const T & prop) const
		{
			*(T *)((UINTPTR)object + m_Offset) = prop;
		}

	private:
		uint32		m_Index;
		uint32		m_Offset;
		uint32		m_Size;
		IMetaType*	m_MetaType;

		friend class MetaStruct;
	};

	class FieldCollection
	{
	public:
		template<class T>
		bool GetProperty(void * object, const char * name, T & prop) const;

		template<class T>
		bool SetProperty(void * object, const char * name, const T & prop) const;

		inline const Array<Field> & GetFields() const { return m_Fields; }

	protected:
		Array<Field> m_Fields;
	};

	template<class T>
	bool FieldCollection::GetProperty(void * object, const char * name, T & prop) const
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
