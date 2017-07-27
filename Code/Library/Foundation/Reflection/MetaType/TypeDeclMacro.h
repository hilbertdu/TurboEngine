// TypeDeclMacro.h
// Type declaration
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_TYPEDECLMACRO_H
#define FOUNDATION_REFLECTION_TYPEDECLMACRO_H


// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/Type.h"
#include "Foundation/Reflection/MetaType/TypeContainer.h"
#include "Foundation/Reflection/Serialization/Serializer.h"
#include "Foundation/Container/HashMap.h"
#include "Foundation/Container/Pair.h"
#include "Foundation/Template/Macros.h"


namespace TReflection
{
#define NAME(X)					Name(TXT(X))
#define TPARAM_KEY_VALUE(...)	__VA_ARGS__

#define REFLECTION_DECLARE_META_BEGIN \
	template<class T> struct MetaDeduce			\
	{											\
		using IsBaseType = std::false_type;		\
	};

#define REFLECTION_DECLARE_META_END

#define REFLECTION_DECLARE_METATYPE(TYPE) \
	class MetaType_##TYPE: public IMetaType				\
	{													\
	public:												\
		MetaType_##TYPE()								\
		{												\
			m_Name = #TYPE;								\
			m_Size = sizeof(TYPE);						\
			m_Serializer = TNEW(Serializer<TYPE>());	\
		}												\
		virtual bool IsBaseType() { return true; }		\
		template<class T> void Register(const T *) {}	\
	};													\
	template<> struct MetaDeduce<TYPE>					\
	{													\
		using IsBaseType = std::true_type;				\
	public:												\
		MetaDeduce() : Name(#TYPE) {}					\
		using MetaType = MetaType_##TYPE;				\
		const char * Name;								\
	};													

#define REFLECTION_DECLARE_METACONTAINERTYPE(TYPE, ...) \
	class MetaType_##TYPE: public IMetaTypeContainer					\
	{																	\
	public:																\
		MetaType_##TYPE()												\
		{																\
			m_Name = #TYPE;												\
		}																\
	};																	\
	template<MACRO_VA_MAPPER(class, __VA_ARGS__)>						\
	struct MetaDeduce<TYPE<__VA_ARGS__>>								\
	{																	\
		using IsBaseType = std::false_type;								\
		using MetaKeyType = MACRO_VA_INDEX(1, __VA_ARGS__, void);		\
		using MetaValueType = MACRO_VA_INDEX(2, __VA_ARGS__, void);		\
	public:																\
		MetaDeduce() : Name(#TYPE) {}									\
		using MetaType = MetaType_##TYPE;								\
		const char * Name;												\
	};

#define REFLECTION_REGISTER_META_BEGIN \
	inline void RegisterAllMetaType(HashMap<int32, IMetaType*>& metaTypes)	\
	{

#define REFLECTION_REGISTER_METATYPE(TYPE, ...) \
		IMetaType * meta_##TYPE = new MetaType_##TYPE();		\
		metaTypes[meta_##TYPE->m_Name.m_Hash] = meta_##TYPE;

#define REFLECTION_REGISTER_META_END \
	}
}

#endif // FOUNDATION_REFLECTION_TYPEDECLMACRO_H
//------------------------------------------------------------------------------
