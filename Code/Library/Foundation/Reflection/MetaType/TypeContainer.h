// TypeContainer.h
// Type containers
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_TYPECONTAINER_H
#define FOUNDATION_REFLECTION_TYPECONTAINER_H


// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/Type.h"
#include "Foundation/Reflection/Container/Iterators.h"
#include "Foundation/Container/Array.h"
#include "Foundation/Container/HashMap.h"
#include "Foundation/Container/List.h"
#include "Foundation/Container/Pair.h"
#include "Foundation/Container/SmartPtr.h"


namespace TReflection
{
	class IContainer : public IType {};

	class IMetaContainer : public IMetaType
	{
	public:
		IMetaContainer() { SetFlag(E_TYPE_CONTAINER); }

		virtual ~IMetaContainer() 
		{ 
			TDELETE_SAFE(m_ReadIterator);
			TDELETE_SAFE(m_WriteIterator);
		}

	public:
		IMetaType*		m_MetaTypeKey{ 0 };
		IMetaType*		m_MetaTypeValue{ 0 };
		IReadIterator*	m_ReadIterator{ 0 };
		IWriteIterator*	m_WriteIterator{ 0 };
	};
}

#endif // FOUNDATION_REFLECTION_TYPECONTAINER_H
//------------------------------------------------------------------------------
