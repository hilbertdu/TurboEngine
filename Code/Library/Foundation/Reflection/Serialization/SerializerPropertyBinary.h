// SerializerBinary.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_SERIALIZERPROPERTY_BINARY_H
#define FOUNDATION_REFLECTION_SERIALIZERPROPERTY_BINARY_H

// Includes
//------------------------------------------------------------------------------

namespace TReflection
{
	// Binary serializer
	//------------------------------------------------------------------------------
	template<>
	struct PropertySerializer<bool, E_SERIALIZER_BINARY>
	{
		static void Save(IOStream* stream, const void * mem) {}
		static void Load(const IOStream* stream, void * mem) {}
	};

	template<>
	struct PropertySerializer<float, E_SERIALIZER_BINARY>
	{
		static void Save(IOStream* stream, const void * mem) {}
		static void Load(const IOStream* stream, void * mem) {}
	};

	template<>
	struct PropertySerializer<double, E_SERIALIZER_BINARY>
	{
		static void Save(IOStream* stream, const void * mem) {}
		static void Load(const IOStream* stream, void * mem) {}
	};

	template<>
	struct PropertySerializer<uint8, E_SERIALIZER_BINARY>
	{
		static void Save(IOStream* stream, const void * mem) {}
		static void Load(const IOStream* stream, void * mem) {}
	};

	template<>
	struct PropertySerializer<uint16, E_SERIALIZER_BINARY>
	{
		static void Save(IOStream* stream, const void * mem) {}
		static void Load(const IOStream* stream, void * mem) {}
	};

	template<>
	struct PropertySerializer<uint32, E_SERIALIZER_BINARY>
	{
		static void Save(IOStream* stream, const void * mem) {}
		static void Load(const IOStream* stream, void * mem) {}
	};

	template<>
	struct PropertySerializer<uint64, E_SERIALIZER_BINARY>
	{
		static void Save(IOStream* stream, const void * mem) {}
		static void Load(const IOStream* stream, void * mem) {}
	};

	template<>
	struct PropertySerializer<int8, E_SERIALIZER_BINARY>
	{
		static void Save(IOStream* stream, const void * mem) {}
		static void Load(const IOStream* stream, void * mem) {}
	};

	template<>
	struct PropertySerializer<int16, E_SERIALIZER_BINARY>
	{
		static void Save(IOStream* stream, const void * mem) {}
		static void Load(const IOStream* stream, void * mem) {}
	};

	template<>
	struct PropertySerializer<int32, E_SERIALIZER_BINARY>
	{
		static void Save(IOStream* stream, const void * mem) {}
		static void Load(const IOStream* stream, void * mem) {}
	};

	template<>
	struct PropertySerializer<int64, E_SERIALIZER_BINARY>
	{
		static void Save(IOStream* stream, const void * mem) {}
		static void Load(const IOStream* stream, void * mem) {}
	};

	template<>
	struct PropertySerializer<AString, E_SERIALIZER_BINARY>
	{
		static void Save(IOStream* stream, const void * mem) {}
		static void Load(const IOStream* stream, void * mem) {}
	};

	template<>
	struct PropertySerializer<Vector2, E_SERIALIZER_BINARY>
	{
		static void Save(IOStream* stream, const void * mem) {}
		static void Load(const IOStream* stream, void * mem) {}
	};
}

//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_SERIALIZERPROPERTY_BINARY_H