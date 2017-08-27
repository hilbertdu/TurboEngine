// Field.h
// Meta type field
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_METHOD_H
#define FOUNDATION_REFLECTION_METHOD_H


// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/Type.h"
#include "Foundation/Pattern/Delegate.h"


namespace TReflection
{
	class Method : public Primitive
	{
	public:
		Method() {}

	public:
		uint32		m_Index;
		AnyDelegate m_Delegate;
		IMetaType*	m_MetaType;
	};

	class MethodCollection
	{
	public:
		template<class T>
		bool GetMethod(const char * name, T& method) const;
		inline const Array<Method> & GetMethods() const { return m_Methods; }

	protected:
		Array<Method> m_Methods;
	};

	template<class T>
	bool MethodCollection::GetMethod(const char * name, T& method) const
	{
		Method * pMethod = m_Methods.FindIf([name](const Method& method) { return method.m_Name == name; });
		if (pMethod)
		{
			method = *(pMethod->m_Delegate.Cast<T>());
			return true;
		}
		return false;
	}
}


#endif // FOUNDATION_REFLECTION_METHOD_H