// TypeDeclMacro.h
// Type declaration
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_TYPEDECLMACRO_H
#define FOUNDATION_REFLECTION_TYPEDECLMACRO_H


// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/Type.h"
#include "Foundation/Reflection/MetaType/TypeDatabase.h"

namespace TReflection
{
#define NAME(X) Name(TXT(X))

#define REFLECTION_DECLARE_META_BEGIN
#define REFLECTION_DECLARE_META_END

#define REFLECTION_DECLARE_METATYPE(type) \
	class MetaType_##type: public MetaType		\
	{											\
	public:										\
		typedef type Innertype;					\
		MetaType_##type() {}					\
		MetaType_##type(const MetaType_##type&) = delete;	\
		MetaType_##type(MetaType_##type&&) = delete;		\
		~MetaType_##type() {}					\
	private:									\
		int m_Size{ sizeof(type) };				\
	};


#define REFLECTION_REGISTER_META_BEGIN \
	void MetaTypeDB::RegisterAll()				\
	{											\

#define REFLECTION_REGISTER_METATYPE(type) \
		MetaType* meta_##type = new MetaType_##type();			\
		m_MetaTypes[meta_##type->m_Name.m_Hash] = meta_##type;	\

#define REFLECTION_REGISTER_META_END \
	}
}

#endif // FOUNDATION_REFLECTION_TYPEDECLMACRO_H
//------------------------------------------------------------------------------
