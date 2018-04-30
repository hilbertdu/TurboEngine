// Object.h
// Base class of all game objects
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_OBJECT_H
#define FOUNDATION_REFLECTION_OBJECT_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/ReflectionMacros.h"

using namespace TReflection;

enum ObjectFlag : uint16
{
	FLAG_OBJECT_NONE		= 0x00,
	FLAG_OBJECT_SERIALIZED	= 0x01
};

class IObject : public RefBase<IObject>
{
public:
	static constexpr MetaFlag s_MetaFlag = E_TYPE_OBJECT;
	explicit IObject();
	virtual ~IObject();

	IObject(const IObject&) = delete;
	IObject& operator=(const IObject&) = delete;

	// todo: new/delete operator

	// flags
	void SetFlag(ObjectFlag flag) { m_Flag = (ObjectFlag)(m_Flag | flag); }
	bool HasFlag(ObjectFlag flag) const { return (m_Flag & flag) != 0; }
	ObjectFlag GetFlag() const { return m_Flag; }

 	// get/set property
	template<class T> bool		SetProperty(const char * name, T&& prop);
 	template<class T> const T	GetProperty(const char * name) const;
 	template<class T> const T * GetPropertyPtr(const char * name) const;
 	template<class T> const T & GetPropertyRef(const char * name) const;		
 
 	// Deduces type membership for this instance
	template<class T> inline bool IsA() const;

	// cast
	template<class T> T * CastTo() const;
 
 	// serialize functions
	virtual void PreSerialize() {}
	virtual void PostSerialize() {}
	virtual void PreDeserialize() {}
	virtual void PostDeserialize() {}

	virtual void Save(IOStream* stream, ISerializer * writer, bool terminal = false) const;
	virtual void Load(const IOStream* stream, ISerializer * reader, bool terminal = false);

private:
	ObjectFlag	m_Flag{ FLAG_OBJECT_NONE };

	TREFLECTION_DECLARE(IObject, IClass)
};

template<class T>
bool IObject::SetProperty(const char * name, T&& prop)
{
}

template<class T>
const T IObject::GetProperty(const char * name) const
{
}

template<class T>
const T * IObject::GetPropertyPtr(const char * name) const
{
}

template<class T>
const T & IObject::GetPropertyRef(const char * name) const
{
}

template<class T> 
inline bool IObject::IsA() const
{
	return CastTo<T>() == this;
}

template<class T> 
T * IObject::CastTo() const
{
	MetaStruct* src = GetMetaTypeV();
	MetaStruct* des = T::GetMetaTypeS();
	while (src)
	{
		if (src == des)
		{
			return (T*)this;
		}
		src = src->m_Super;
	}
	return nullptr;
}

//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_OBJECT_H