// SerializerText.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_SERIALIZERTEXT_H
#define FOUNDATION_REFLECTION_SERIALIZERTEXT_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/FileIO/IOStream.h"

namespace TReflection
{
	template<class T>
	struct TextSerializer;

	template<>
	struct TextSerializer<bool>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct TextSerializer<float>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct TextSerializer<double>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct TextSerializer<uint8>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct TextSerializer<uint16>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct TextSerializer<uint32>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct TextSerializer<uint64>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct TextSerializer<int8>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct TextSerializer<int16>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct TextSerializer<int32>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct TextSerializer<int64>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct TextSerializer<AString>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};
}


//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_SERIALIZER_H