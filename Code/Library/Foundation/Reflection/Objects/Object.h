// Object.h
// Base class of all game objects
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_OBJECT_H
#define FOUNDATION_REFLECTION_OBJECT_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/MetaType/TypeClass.h"
#include "Foundation/Reflection/MetaType/TypeDecl.h"


namespace TReflection
{
	class IObject : public IClass
	{
	public:
		IObject() {}
		virtual ~IObject() {}

		// new/delete operator

// 		// meta
// 		virtual IObject* Create() const;
// 		virtual const MetaClass* GetMetaType();
// 		
// 		// get/set property
// 		template<class T>
// 		bool SetProperty(const char * name, T&& prop) const;
// 
// 		template<class T>
// 		const T GetPropertyValue(const char * name) const;
// 
// 		template<class T>
// 		const T* GetPropertyPointer(const char * name) const;
// 
// 		template<class T>
// 		const T& GetPropertyReference(const char * name) const;
// 
// 		// Deduces type membership for this instance
// 		bool IsA(const MetaClass* type) const;
// 
// 		// serialize/deserialize
// 		virtual void PreSerialize();
// 		virtual void PostSerialize();
// 		virtual void PreDeserialize();
// 		virtual void PostDeserialize();
	};
	//REFLECTION_META_DEDUCE(IObject, IObject::MetaType)
}


//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_OBJECT_H