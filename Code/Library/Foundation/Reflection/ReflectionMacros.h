// ReflectionMacros.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_MACROS_H
#define FOUNDATION_REFLECTION_MACROS_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/TypeDecl.h"
#include "Foundation/Reflection/MetaType/TypeClass.h"
#include "Foundation/Reflection/MetaType/TypeDatabase.h"

// Macros
//------------------------------------------------------------------------------
#define _TREFLECTION_DECLARE(OBJECT, BASE) \
	public:															\
		using IType = TReflection::IType;							\
		using MetaStruct = TReflection::MetaStruct;					\
		using MetaClass = TReflection::MetaClass;					\
		using MetaTypeDB = TReflection::MetaTypeDB;					\
		using MetaType = BASE::MetaType;							\
		using BaseType = BASE;										\
		using ObjectType = OBJECT;									\
		static inline IType* Create();								\
		static inline IType* Create2();								\
		static inline void RegisterMetaType();						\
		static inline void RegisterProperty();						\
		static inline void BindReflectionInfo();					\
		static inline MetaType * GetMetaTypeS();							\
		virtual inline const TReflection::IMetaType* GetMetaTypeV() const;	\
		OBJECT(TReflection::IMetaType * UNUSED(x)) {}

#define _TREFLECTION_BEGIN(OBJECT) \
	namespace TReflection										\
	{															\
		REFLECTION_DECLARE_METAOBJECT(OBJECT, OBJECT::MetaType)	\
	}															\
	_TREFLECTION_BEGIN_COMMON(OBJECT)

#define _TREFLECTION_BEGIN_COMMON(OBJECT) \
	/*static*/ inline void OBJECT::RegisterMetaType()													\
	{																									\
		MetaType* metaType = static_cast<MetaType*>(MetaTypeDB::Instance().CreateMetaType<OBJECT>());	\
		metaType->m_Name = TXT(#OBJECT);																\
		metaType->m_Super = static_cast<MetaStruct*>(MetaTypeDB::Instance().GetMetaType<BaseType>());	\
		metaType->m_Creator = &OBJECT::Create;															\
		metaType->m_Creator2 = &OBJECT::Create2;														\
		metaType->SetFlag(BaseType::s_MetaFlag);														\
	}																									\
	_TREFLECT_STRUCT_COMMON(OBJECT)

#define _TREFLECT_STRUCT_COMMON(OBJECT) \
	/*static*/ inline OBJECT::IType* OBJECT::Create()							\
	{																			\
		return (OBJECT::IType*)(TNEW(OBJECT));									\
	}																			\
	/*static*/ inline OBJECT::IType* OBJECT::Create2()							\
	{																			\
		return (OBJECT::IType*)(TNEW(OBJECT(OBJECT::GetMetaTypeS())));			\
	}																			\
	/*static*/ OBJECT::MetaType * OBJECT::GetMetaTypeS()						\
	{																			\
		return (OBJECT::MetaType*)MetaTypeDB::Instance().GetMetaType<OBJECT>();	\
	}																			\
	const TReflection::IMetaType* OBJECT::GetMetaTypeV() const					\
	{																			\
		return MetaTypeDB::Instance().GetMetaType<OBJECT>();					\
	}																			\
	/*static*/ inline void OBJECT::RegisterProperty()							\
	{

#define TREFLECT_FIELD(FIELD, FNAME, ...)	\
		GetMetaTypeS()->AddField(&((ObjectType*)0)->FIELD, TXT(FNAME));	\
		GetMetaTypeS()->TopField().AddAttribute(__VA_ARGS__);

#define TREFLECT_METHOD(METHOD, FNAME)	\
		GetMetaTypeS()->AddMethod(&ObjectType::METHOD, TXT(FNAME));	\

#define _TREFLECTION_END(OBJECT)	\
	}													\
	/*static*/ inline void OBJECT::BindReflectionInfo()	\
	{													\
		RegisterMetaType();								\
		RegisterProperty();								\
	}

#define BIND_REFLECTION(OBJECT) OBJECT::BindReflectionInfo()

#define ATTRIBUTE(...) MACRO_VA_MAPPER(TReflection::, __VA_ARGS__)

#define TREFLECTION_DECLARE(OBJECT, BASE)	_TREFLECTION_DECLARE(OBJECT, BASE)
#define TREFLECT_BEGIN(OBJECT)				_TREFLECTION_BEGIN(OBJECT)
#define TREFLECT_END(OBJECT)				_TREFLECTION_END(OBJECT)

#endif // FOUNDATION_REFLECTION_MACROS_H