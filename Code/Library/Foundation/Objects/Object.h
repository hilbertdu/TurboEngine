// Object.h
// Base class of all game objects
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_OBJECT_H
#define FOUNDATION_REFLECTION_OBJECT_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/Reflection.h"
#include "Foundation/Objects/RefObject.h"
#include "Foundation/Objects/ObjectFlag.h"


class IObject : public RefObject<IObject>
{
public:
	static constexpr TReflection::MetaFlag s_MetaFlag = TReflection::E_TYPE_OBJECT;
	using MetaType = TReflection::MetaClass;
	explicit IObject() {}
	virtual ~IObject() {}

	IObject(const IObject&) = delete;
	IObject& operator=(const IObject&) = delete;

	// todo: new/delete operator

	// name & flags
	inline const TReflection::Name & GetName() const { };//return GetMetaTypeV()->GetName(); }
	inline void SetFlag(ObjectFlag flag) { m_Flag = (ObjectFlag)(m_Flag | flag); }
	inline bool HasFlag(ObjectFlag flag) const { return (m_Flag & flag) != 0; }
	inline ObjectFlag GetFlag() const { return (ObjectFlag)m_Flag; }

 	// get/set property
	template<class T> inline bool		SetProperty(const char * name, T&& prop);
 	template<class T> inline const T	GetProperty(const char * name) const;
 	template<class T> inline const T *	GetPropertyPtr(const char * name) const;
 	template<class T> inline const T &	GetPropertyRef(const char * name) const;
 
 	// Deduces type membership for this instance
	template<class T> inline bool IsA() const;

	// cast
	template<class T> T * CastTo() const;
 
 	// serialize functions	
	virtual void PreSerialize() {}
	virtual void PostSerialize() {}
	virtual void PreDeserialize() {}
	virtual void PostDeserialize() {}

	inline void Save(IOStream* stream, TReflection::ISerializer * writer, bool terminal = false) const;
	inline void Load(const IOStream* stream, TReflection::ISerializer * reader, bool terminal = false);

private:
	uint16	m_Flag{ FLAG_OBJECT_NONE };

	TREFLECTION_DECLARE(IObject, TReflection::IClass)
};

#include "Foundation/Objects/Object.inl"

//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_OBJECT_H