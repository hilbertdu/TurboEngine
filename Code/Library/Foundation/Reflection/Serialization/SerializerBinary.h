// SerializerBinary.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_SERIALIZERBINARY_H
#define FOUNDATION_REFLECTION_SERIALIZERBINARY_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/FileIO/IOStream.h"

namespace TReflection
{
	template<class T>
	struct BinarySerializer;

	template<>
	struct BinarySerializer<bool>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct BinarySerializer<float>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct BinarySerializer<double>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct BinarySerializer<uint8>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct BinarySerializer<uint16>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct BinarySerializer<uint32>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct BinarySerializer<uint64>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct BinarySerializer<int8>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct BinarySerializer<int16>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct BinarySerializer<int32>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct BinarySerializer<int64>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	struct BinarySerializer<AString>
	{
		static void Save(IOStream & stream, const void * mem) {}
		static void Load(const IOStream & stream, void * mem) {}
	};
}


//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_SERIALIZERBINARY_H