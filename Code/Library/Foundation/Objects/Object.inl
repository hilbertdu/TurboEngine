// Object.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/Serialization/Serializer.h"


template<class T>
bool IObject::SetProperty(const char * name, T&& prop)
{
	return ((TReflection::MetaStruct*)GetMetaTypeV())->SetProperty(this, name, std::forward<T>(prop));
}

template<class T>
const T IObject::GetProperty(const char * name) const
{
	T prop;
	((TReflection::MetaStruct*)GetMetaTypeV())->GetProperty(this, name, prop);
	return prop;
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
	const MetaType* src = static_cast<const MetaType*>(GetMetaTypeV());
	const MetaType* des = T::GetMetaTypeS();
	while (src)
	{
		if (src == des)
		{
			return (T*)this;
		}
		src = static_cast<const MetaType*>(src->m_Super);
	}
	return nullptr;
}

void IObject::Save(IOStream* stream, TReflection::ISerializer * writer, bool terminal) const
{
	writer->Save(stream, this, GetMetaTypeV(), terminal);
}

void IObject::Load(const IOStream* stream, TReflection::ISerializer * reader, bool terminal)
{
	reader->Load(stream, this, GetMetaTypeV(), terminal);
}

//------------------------------------------------------------------------------