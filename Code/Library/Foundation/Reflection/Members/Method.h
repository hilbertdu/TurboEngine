// Field.h
// Meta type field
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_METHOD_H
#define FOUNDATION_REFLECTION_METHOD_H


// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/Type.h"
#include "Foundation/Env/Assert.h"


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
		const AnyDelegate & GetMethod(const char * name) const;
		inline const Array<Method> & GetMethods() const { return m_Methods; }

	protected:
		Array<Method> m_Methods;
	};

	const AnyDelegate & MethodCollection::GetMethod(const char * name) const
	{
		Method * iter = m_Methods.FindIf([name](const Method method) { return method.m_Name == name; });
		return iter == m_Methods.End() ? AnyDelegate::s_EmptyDelegate : iter->GetMethod();
	}
}


#endif // FOUNDATION_REFLECTION_METHOD_H