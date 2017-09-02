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
#include "Foundation/Reflection/Serialization/SerializerProperty.h"
#include "Foundation/Reflection/Container/Iterators.h"
#include "Foundation/Container/HashMap.h"
#include "Foundation/Template/Macros.h"


namespace TReflection
{
#define NAME(X)				Name(TXT(X))
#define KEY(K)				K
#define VALUE(V)			V
#define TEMPLATE(...)		__VA_ARGS__

#define REFLECTION_DECLARE_META_BEGIN \
	template<class T>								\
	class MetaType : public IMetaType				\
	{												\
	public:											\
		using Type = T;								\
		using IsContainerType = std::false_type;	\
	};

#define REFLECTION_DECLARE_META_END

#define REFLECTION_DECLARE_METABASE(TYPE) \
	template<>										\
	class MetaType<TYPE> : public IMetaType			\
	{												\
	public:											\
		using Type = TYPE;							\
		using IsContainerType = std::false_type;	\
		MetaType()  {								\
			m_Name = #TYPE;							\
			m_Size = sizeof(TYPE);					\
			SetFlag(E_TYPE_BASE);					\
		}											\
		void* Create() { return TNEW(TYPE);	}		\
	};

#define REFLECTION_DECLARE_METACONTAINERTYPE(TYPE, KEY_IDX, VALUE_IDX, ...) \
	template<MACRO_VA_MAPPER(class, __VA_ARGS__)>							\
	class MetaType<TYPE<__VA_ARGS__>> : public IMetaContainer				\
	{																		\
	public:																	\
		using Type = TYPE<__VA_ARGS__>;										\
		using IsContainerType = std::true_type;								\
		using MetaKeyType = MACRO_VA_INDEX(KEY_IDX, __VA_ARGS__);			\
		using MetaValueType = MACRO_VA_INDEX(VALUE_IDX, __VA_ARGS__);		\
		MetaType() {														\
			m_Name = #TYPE;													\
			m_Name += MetaType<MetaKeyType>().m_Name;						\
			m_Name += MetaType<MetaValueType>().m_Name;						\
			m_ReadIterator = TNEW(TYPE##ReadIterator<__VA_ARGS__>);			\
			m_WriteIterator = TNEW(TYPE##WriteIterator<__VA_ARGS__>);		\
		}																	\
		void* Create() { return TNEW(TYPE<__VA_ARGS__>); }					\
	};

#define REFLECTION_DECLARE_METAOBJECT(OBJECT, BASE) \
	template<> class MetaType<OBJECT> : public BASE		\
	{													\
	public:												\
		using Type = OBJECT;							\
		using IsContainerType = std::false_type;		\
		MetaType()  {									\
			m_Name = #OBJECT;							\
			m_Size = sizeof(OBJECT);					\
		}												\
		void* Create() { return TNEW(OBJECT); }			\
	};

#define REFLECTION_REGISTER_META_BEGIN \
	inline void RegisterAllMetaType(HashMap<int32, IMetaType*>& metaTypes)	\
	{

#define REFLECTION_REGISTER_METATYPE(TYPE, ...) \
		IMetaType * meta_##TYPE = TNEW(MetaType<TYPE>());		\
		metaTypes[meta_##TYPE->m_Name.m_Hash] = meta_##TYPE;

#define REFLECTION_REGISTER_META_END \
	}


#define REFLECTION_REGISTER_SERIALIZER_BEGIN \
	inline void RegisterAllSerializer(HashMap<int32, IMetaType*>& metaTypes)	\
	{

#define REFLECTION_REGISTER_SERIALIZER(TYPE, STYPE) \
		metaTypes[Name(#TYPE).m_Hash]->SetLoad(STYPE, &PropertySerializer<TYPE, STYPE>::Load);	\
		metaTypes[Name(#TYPE).m_Hash]->SetSave(STYPE, &PropertySerializer<TYPE, STYPE>::Save);

#define REFLECTION_REGISTER_SERIALIZER_END \
	}
}


#endif // FOUNDATION_REFLECTION_TYPEDECLMACRO_H
//------------------------------------------------------------------------------
