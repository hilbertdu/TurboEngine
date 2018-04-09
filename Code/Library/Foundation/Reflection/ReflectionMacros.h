// ReflectionMacros.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_MACROS_H
#define FOUNDATION_REFLECTION_MACROS_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/Reflection.h"
#include "Foundation/Reflection/Attributes/Attributes.h"


#define _TREFLECTION_DECLARE(OBJECT, BASE) \
	public:												\
		using IType = TReflection::IType;				\
		using MetaStruct = TReflection::MetaStruct;		\
		using MetaClass = TReflection::MetaClass;		\
		using MetaTypeDB = TReflection::MetaTypeDB;		\
		using MetaType = BASE::MetaType;				\
		using BaseType = BASE;							\
		using ObjectType = OBJECT;						\
		static IType* Create();							\
		static IType* Create2();						\
		static void RegisterMetaType();					\
		static void RegisterProperty();					\
		static void BindReflectionInfo();				\
		inline static MetaType * GetMetaTypeS();		\
		virtual const IMetaType* GetMetaTypeV() const;	\
		OBJECT(IMetaType * UNUSED(x)) {}

#define _TREFLECTION_BEGIN(OBJECT) \
	namespace TReflection										\
	{															\
		REFLECTION_DECLARE_METAOBJECT(OBJECT, OBJECT::MetaType)	\
	}															\
	_TREFLECTION_BEGIN_COMMON(OBJECT)

#define _TREFLECTION_BEGIN_COMMON(OBJECT) \
	/*static*/ void OBJECT::RegisterMetaType()															\
	{																									\
		MetaType* metaType = (MetaType*)MetaTypeDB::Instance().CreateMetaType<OBJECT>();				\
		metaType->m_Name = TXT(#OBJECT);																\
		metaType->m_Super = static_cast<MetaStruct*>(MetaTypeDB::Instance().GetMetaType<BaseType>());	\
		metaType->m_Creator = &OBJECT::Create;															\
		metaType->m_Creator2 = &OBJECT::Create2;														\
		metaType->SetFlag(BaseType::s_MetaFlag);														\
	}																									\
	_TREFLECT_STRUCT_COMMON(OBJECT)

#define _TREFLECT_STRUCT_COMMON(OBJECT) \
	/*static*/ OBJECT::IType* OBJECT::Create()									\
	{																			\
		return (OBJECT::IType*)(TNEW(OBJECT));									\
	}																			\
	/*static*/ OBJECT::IType* OBJECT::Create2()									\
	{																			\
		return (OBJECT::IType*)(TNEW(OBJECT(OBJECT::GetMetaTypeS())));			\
	}																			\
	/*static*/ inline OBJECT::MetaType * OBJECT::GetMetaTypeS()					\
	{																			\
		return (OBJECT::MetaType*)MetaTypeDB::Instance().GetMetaType<OBJECT>();	\
	}																			\
	/*virtual*/ const IMetaType * OBJECT::GetMetaTypeV() const					\
	{																			\
		return MetaTypeDB::Instance().GetMetaType<OBJECT>();					\
	}																			\
	/*static*/ void OBJECT::RegisterProperty()									\
	{

#define TREFLECT_FIELD(FIELD, FNAME, ...)	\
		GetMetaTypeS()->AddField(&((ObjectType*)0)->FIELD, TXT(FNAME));	\
		GetMetaTypeS()->TopField().AddAttribute(__VA_ARGS__);

#define TREFLECT_METHOD(METHOD, FNAME)	\
		GetMetaTypeS()->AddMethod(&ObjectType::METHOD, TXT(FNAME));	\

#define _TREFLECTION_END(OBJECT)	\
	}												\
	/*static*/ void OBJECT::BindReflectionInfo()	\
	{												\
		RegisterMetaType();							\
		RegisterProperty();							\
	}

#define BIND_REFLECTION(OBJECT) OBJECT::BindReflectionInfo()

#define ATTRIBUTE(...) MACRO_VA_MAPPER(TReflection::, __VA_ARGS__)

#define TREFLECTION_DECLARE(OBJECT, BASE)	_TREFLECTION_DECLARE(OBJECT, BASE)
#define TREFLECT_BEGIN(OBJECT)				_TREFLECTION_BEGIN(OBJECT)
#define TREFLECT_END(OBJECT)				_TREFLECTION_END(OBJECT)

#endif // FOUNDATION_REFLECTION_MACROS_H