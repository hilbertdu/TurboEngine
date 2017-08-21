// ReflectionMacros.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_MACROS_H
#define FOUNDATION_REFLECTION_MACROS_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/Reflection.h"


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
		static void RegisterMetaType();					\
		static void RegisterProperty();					\
		static void BindReflectionInfo();				\
		inline static MetaType * GetMetaTypeS();

#define _TREFLECTION_BEGIN(OBJECT) \
	/*static*/ void OBJECT::RegisterMetaType()														\
	{																								\
		MetaType * info = GetMetaTypeS();															\
		info->m_Name = TXT(#OBJECT);																\
		info->m_Super = static_cast<MetaStruct*>(MetaTypeDB::Instance().GetMetaType<BaseType>());	\
		info->m_Creator = &OBJECT::Create;															\
		MetaTypeDB::Instance().Register(info);														\
	}																								\
	_TREFLECT_STRUCT_COMMON(OBJECT)

#define _TREFLECT_STRUCT_COMMON(OBJECT) \
	/*static*/ OBJECT::IType * OBJECT::Create()						\
	{																\
		return (OBJECT::IType*)(TNEW(OBJECT));						\
	}																\
	/*static*/ inline OBJECT::MetaType * OBJECT::GetMetaTypeS()		\
	{																\
		static MetaType * info = TNEW(MetaType);					\
		return info;												\
	}																\
	/*static*/ void OBJECT::RegisterProperty()						\
	{

#define TREFLECT_FIELD(FIELD, FNAME)	\
		GetMetaTypeS()->AddField(&((ObjectType*)0)->FIELD, TXT(FNAME));	\

#define TREFLECT_METHOD(METHOD, FNAME)	\
		GetMetaTypeS()->AddMethod(&ObjectType::METHOD, TXT(FNAME));	\

#define _TREFLECTION_END(OBJECT)	\
	}														\
	/*static*/ void OBJECT::BindReflectionInfo()			\
	{														\
		RegisterMetaType();									\
		RegisterProperty();									\
	}														\
	namespace TReflection									\
	{														\
		REFLECTION_META_DEDUCE(OBJECT, OBJECT::MetaType)	\
	}

#define TREFLECTION_DECLARE(OBJECT, BASE)	_TREFLECTION_DECLARE(OBJECT, BASE)
#define TREFLECT_BEGIN(OBJECT)				_TREFLECTION_BEGIN(OBJECT)
#define TREFLECT_END(OBJECT)				_TREFLECTION_END(OBJECT)

#endif // FOUNDATION_REFLECTION_MACROS_H