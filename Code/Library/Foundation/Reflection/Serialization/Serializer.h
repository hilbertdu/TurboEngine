// Serializer.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_SERIALIZER_H
#define FOUNDATION_REFLECTION_SERIALIZER_H

// Includes
//------------------------------------------------------------------------------

class IOStream;

namespace TReflection
{
	class IMetaType;

	enum SerializeType : uint8
	{
		E_SERIALIZER_TEXT = 0,
		//E_SERIALIZER_BINARY,
		E_SERIALIZER_COUNT
	};

	typedef void(*SerializerSave)(IOStream* stream, const void * mem);
	typedef void(*SerializerLoad)(const IOStream* stream, void * mem);

	class Serializer
	{
	public:
		SerializerSave m_SaveFunc[E_SERIALIZER_COUNT]{ 0 };
		SerializerLoad m_LoadFunc[E_SERIALIZER_COUNT]{ 0 };
	};

	class ISerializer
	{
	public:
		virtual void Save(IOStream* stream, const void * object, const IMetaType * objectType, bool terminal = false) = 0;
		virtual void Load(const IOStream* stream, void * object, const IMetaType * objectType, bool terminal = false) = 0;
	};
}


#endif // FOUNDATION_REFLECTION_SERIALIZER_H

