// Serializer.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_SERIALIZERS_H
#define FOUNDATION_REFLECTION_SERIALIZERS_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/FileIO/IOStream.h"
#include "Foundation/Reflection/Serialization/SerializerText.h"
#include "Foundation/Reflection/Serialization/SerializerBinary.h"

namespace TReflection
{
	enum SerializeType : uint8
	{
		E_SERIALIZER_TEXT = 0,
		E_SERIALIZER_BINARY,
		E_SERIALIZER_COUNT
	};

	typedef void(*SerializerSave)(IOStream & stream, const void * mem);
	typedef void(*SerializerLoad)(const IOStream & stream, void * mem);

	class IMetaType;
	void Save(IOStream & stream, const void * object, const IMetaType * objectType, SerializeType sType);
	void Load(const IOStream & stream, void * object, const IMetaType * objectType, SerializeType sType);

	class Serializer
	{
	public:
		SerializerSave m_SaveFunc[E_SERIALIZER_COUNT]{ 0 };
		SerializerLoad m_LoadFunc[E_SERIALIZER_COUNT]{ 0 };
	};
}


#endif // FOUNDATION_REFLECTION_SERIALIZERS_H

