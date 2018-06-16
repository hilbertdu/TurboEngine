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
#include "Foundation/Math/Math.h"
#include "Foundation/Container/HashMap.h"
#include "Foundation/Template/Macros.h"
#include "Foundation/Template/CompileHelper.h"
#include <type_traits>


GENERATE_HAS_MEMBER(BindReflectionInfo)

#define RFL_NAME(X)							Name(TXT(X))
#define RFL_KEY(K)							K
#define RFL_VALUE(V)						V
#define RFL_TEMPLATE(...)					__VA_ARGS__
#define RFL_STATIC_CONST_STR(NAME, VALUE)	static constexpr const char* NAME = #VALUE;
#define NRFL_STATIC_CONST_STR(NAME, VALUE)	RFL_STATIC_CONST_STR(NAME, VALUE);

#define RFL_STATIC_NAME(NAME) \
	static const StackName& StaticName(const char* name = nullptr)	\
	{																\
		static StackName sName = #NAME;								\
		if (name) { sName = name; }									\
		return sName;												\
	}

#define REFLECTION_DECLARE_META_BEGIN \
	template<class T>																\
	class MetaType : public IMetaType												\
	{																				\
	public:																			\
		static_assert(!HasMember_BindReflectionInfo<T>::value, "Error MetaType");	\
		StaticPrint("Declare MetaType<>")											\
		using Type = T;																\
		using IsVoid = std::true_type;												\
		RFL_STATIC_NAME(void)														\
	};

#define REFLECTION_DECLARE_META_END

#define REFLECTION_DECLARE_METABASE(TYPE) \
	template<>																			\
	class MetaType<TYPE> : public IMetaType												\
	{																					\
	public:																				\
		StaticPrint("Declare MetaType<" #TYPE ">")										\
		using Type = TYPE;																\
		using IsVoid = std::false_type;													\
		MetaType()  {																	\
			m_Name = #TYPE;																\
			m_Size = sizeof(Type);														\
			SetFlag(E_TYPE_BASE);														\
		}																				\
		virtual void* Create() const { return TNEW(Type); }								\
		virtual void  Destory(void* ptr) const { TDELETE(static_cast<Type*>(ptr)); }	\
		RFL_STATIC_NAME(TYPE)															\
	};

#define REFLECTION_DECLARE_METACONTAINERTYPE(TYPE, KEY_IDX, VALUE_IDX, ...) \
	template<MACRO_VA_MAPPER(class, __VA_ARGS__)>																\
	class MetaType<TYPE<__VA_ARGS__>> : public IMetaContainer													\
	{																											\
	public:																										\
		StaticPrint("Declare Container MetaType<" MACRO_STR(TYPE) ">")											\
		StaticPrint("Declare Container Key MetaType<" MACRO_STR(MACRO_VA_INDEX(KEY_IDX, __VA_ARGS__)) ">")		\
		StaticPrint("Declare Container Value MetaType<" MACRO_STR(MACRO_VA_INDEX(VALUE_IDX, __VA_ARGS__)) ">")	\
		using Type = TYPE<__VA_ARGS__>;																			\
		using IsVoid = std::false_type;																			\
		using MetaKeyType = MACRO_VA_INDEX(KEY_IDX, __VA_ARGS__);												\
		using MetaValueType = MACRO_VA_INDEX(VALUE_IDX, __VA_ARGS__);											\
		MetaType() {																							\
			using KeyType = std::remove_pointer<MetaKeyType>::type;												\
			using ValueType = std::remove_pointer<MetaValueType>::type;											\
			m_MetaTypeKey = MetaTypeDB::Instance().ObtainMetaType<KeyType>();									\
			m_MetaTypeValue = MetaTypeDB::Instance().ObtainMetaType<ValueType>();								\
			m_ReadIterator = TNEW(TYPE##ReadIterator<__VA_ARGS__>);												\
			m_WriteIterator = TNEW(TYPE##WriteIterator<__VA_ARGS__>);											\
			m_Size = sizeof(Type);																				\
			m_Name = #TYPE;																						\
			m_Name += MetaType<KeyType>::StaticName();															\
			m_Name += std::is_pointer<MetaKeyType>::value ? "*" : "";											\
			m_Name += MetaType<ValueType>::StaticName();														\
			m_Name += std::is_pointer<MetaValueType>::value ? "*" : "";											\
			StaticName(m_Name.m_Name.Get());																	\
		}																										\
		virtual bool IsKeyPointer() const { return std::is_pointer<MetaKeyType>::value; }						\
		virtual bool IsValuePointer() const { return std::is_pointer<MetaValueType>::value; }					\
		virtual void* Create() const { return TNEW(Type); }														\
		virtual void  Destory(void* ptr) const { TDELETE(static_cast<Type*>(ptr)); }							\
		RFL_STATIC_NAME(void)																					\
	};

#define REFLECTION_DECLARE_METAOBJECT(OBJECT, BASE) \
	template<> class MetaType<OBJECT> : public BASE										\
	{																					\
	public:																				\
		StaticPrint("Declare MetaType<" #OBJECT ">, Base: " #BASE)						\
		using Type = OBJECT;															\
		using IsVoid = std::false_type;													\
		MetaType()  {																	\
			m_Name = #OBJECT;															\
			m_Size = sizeof(Type);														\
		}																				\
		virtual void* Create() const { return TNEW(Type); }								\
		virtual void  Destory(void* ptr) const { TDELETE(static_cast<Type*>(ptr)); }	\
		RFL_STATIC_NAME(OBJECT)															\
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


//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_TYPEDECLMACRO_H
