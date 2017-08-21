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

		inline const AnyDelegate & GetMethod() const { return m_Delegate; }

	private:
		uint32		m_Index;
		AnyDelegate m_Delegate;
		IMetaType*	m_MetaType;		

		friend class MethodCollection;
		friend class MetaStruct;
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
			method = *(pMethod->GetMethod().Cast<T>());
			return true;
		}
		return false;
	}
}


#endif // FOUNDATION_REFLECTION_METHOD_H