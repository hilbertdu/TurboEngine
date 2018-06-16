// SerializerText.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_SERIALIZERPROPERTY_TEXT_H
#define FOUNDATION_REFLECTION_SERIALIZERPROPERTY_TEXT_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/FileIO/IOStream.h"


namespace TReflection
{
	// Text serializer
	//------------------------------------------------------------------------------
	template<>
	struct PropertySerializer<bool, E_SERIALIZER_TEXT>
	{
		static void Save(IOStream* stream, const void * mem)
		{
			AString content;
			content.Format("%s", *(bool*)mem ? "true" : "false");
			stream->Write(content.Get(), content.GetLength());
			stream->Write("\n", 1);
		}
		static void Load(const IOStream* stream, void * mem)
		{
			AString content;
			stream->ReadLine(content);
			*(bool*)mem = content == "true" ? true : false;
		}
	};

	template<>
	struct PropertySerializer<float, E_SERIALIZER_TEXT>
	{
		static void Save(IOStream* stream, const void * mem)
		{
			AString content;
			content.Format("%1.2f", *(float*)mem);
			stream->Write(content.Get(), content.GetLength());
			stream->Write("\n", 1);
		}
		static void Load(const IOStream* stream, void * mem)
		{
			AString content;
			stream->ReadLine(content);
			sscanf_s(content.Get(), "%f", (float*)mem);
		}
	};

	template<>
	struct PropertySerializer<double, E_SERIALIZER_TEXT>
	{
		static void Save(IOStream* stream, const void * mem)
		{
			AString content;
			content.Format("%1.2lf", *(double*)mem);
			stream->Write(content.Get(), content.GetLength());
			stream->Write("\n", 1);
		}
		static void Load(const IOStream* stream, void * mem)
		{
			AString content;
			stream->ReadLine(content);
			sscanf_s(content.Get(), "%lf", (double*)mem);
		}
	};

	template<>
	struct PropertySerializer<uint8, E_SERIALIZER_TEXT>
	{
		static void Save(IOStream* stream, const void * mem)
		{
			AString content;
			content.Format("%u", *(uint8*)mem);
			stream->Write(content.Get(), content.GetLength());
			stream->Write("\n", 1);
		}
		static void Load(const IOStream* stream, void * mem)
		{
			AString content;
			stream->ReadLine(content);

			uint32 u32;
			sscanf_s(content.Get(), "%u", &u32);
			ASSERT(u32 <= 255);
			*(uint8*)mem = (uint8)u32;
		}
	};

	template<>
	struct PropertySerializer<uint16, E_SERIALIZER_TEXT>
	{
		static void Save(IOStream* stream, const void * mem)
		{
			AString content;
			content.Format("%u", *(uint16*)mem);
			stream->Write(content.Get(), content.GetLength());
			stream->Write("\n", 1);
		}
		static void Load(const IOStream* stream, void * mem)
		{
			AString content;
			stream->ReadLine(content);

			uint32 u32;
			sscanf_s(content.Get(), "%u", &u32);
			ASSERT(u32 <= 65536);
			*(uint16*)mem = (uint16)u32;
		}
	};

	template<>
	struct PropertySerializer<uint32, E_SERIALIZER_TEXT>
	{
		static void Save(IOStream* stream, const void * mem)
		{
			AString content;
			content.Format("%u", *(uint32*)mem);
			stream->Write(content.Get(), content.GetLength());
			stream->Write("\n", 1);
		}
		static void Load(const IOStream* stream, void * mem)
		{
			AString content;
			stream->ReadLine(content);

			sscanf_s(content.Get(), "%u", (uint32*)mem);
		}
	};

	template<>
	struct PropertySerializer<uint64, E_SERIALIZER_TEXT>
	{
		static void Save(IOStream* stream, const void * mem)
		{
			AString content;
#if defined(__LINUX__)
			content.Format("%lu", *(uint64*)mem);
#else
			content.Format("%llu", *(uint64*)mem);
#endif
			stream->Write(content.Get(), content.GetLength());
			stream->Write("\n", 1);
		}
		static void Load(const IOStream* stream, void * mem)
		{
			AString content;
			stream->ReadLine(content);

#if defined(__LINUX__)
			sscanf_s(content.Get(), "%lu", (uint64*)mem);
#else
			sscanf_s(content.Get(), "%llu", (uint64*)mem);
#endif
		}
	};

	template<>
	struct PropertySerializer<int8, E_SERIALIZER_TEXT>
	{
		static void Save(IOStream* stream, const void * mem)
		{
			AString content;
			content.Format("%i", *(int8*)mem);
			stream->Write(content.Get(), content.GetLength());
			stream->Write("\n", 1);
		}
		static void Load(const IOStream* stream, void * mem)
		{
			AString content;
			stream->ReadLine(content);

			int32 i32;
			sscanf_s(content.Get(), "%i", &i32);
			ASSERT(i32 >= -128 && i32 <= 127);
			*(int8*)mem = (int8)i32;
		}
	};

	template<>
	struct PropertySerializer<int16, E_SERIALIZER_TEXT>
	{
		static void Save(IOStream* stream, const void * mem)
		{
			AString content;
			content.Format("%i", *(int16*)mem);
			stream->Write(content.Get(), content.GetLength());
			stream->Write("\n", 1);
		}
		static void Load(const IOStream* stream, void * mem)
		{
			AString content;
			stream->ReadLine(content);

			int32 i32;
			sscanf_s(content.Get(), "%i", &i32);
			ASSERT(i32 >= -32768 && i32 <= 32767);
			*(int16*)mem = (int16)i32;
		}
	};

	template<>
	struct PropertySerializer<int32, E_SERIALIZER_TEXT>
	{
		static void Save(IOStream* stream, const void * mem)
		{
			AString content;
			content.Format("%i", *(int32*)mem);
			stream->Write(content.Get(), content.GetLength());
			stream->Write("\n", 1);
		}
		static void Load(const IOStream* stream, void * mem)
		{
			AString content;
			stream->ReadLine(content);

			sscanf_s(content.Get(), "%i", (int32*)mem);
		}
	};

	template<>
	struct PropertySerializer<int64, E_SERIALIZER_TEXT>
	{
		static void Save(IOStream* stream, const void * mem)
		{
			AString content;
			content.Format("%lld", *(int64*)mem);
			stream->Write(content.Get(), content.GetLength());
			stream->Write("\n", 1);
		}
		static void Load(const IOStream* stream, void * mem)
		{
			AString content;
			stream->ReadLine(content);

#if defined(__LINUX__)
			sscanf_s(content.Get(), "%ld", (int64*)mem);
#else
			sscanf_s(content.Get(), "%lld", (int64*)mem);
#endif
		}
	};

	template<>
	struct PropertySerializer<AString, E_SERIALIZER_TEXT>
	{
		static void Save(IOStream* stream, const void * mem)
		{
			const AString* str = (const AString*)mem;

			stream->Write("\"", 1);
			stream->Write(str->Get(), str->GetLength());
			stream->Write("\"", 1);
			stream->Write("\n", 1);
		}
		static void Load(const IOStream* stream, void * mem)
		{
			AString content;
			stream->ReadLine(content);
			ASSERT(content.GetLength() >= 2);

			AString* str = (AString*)mem;
			str->Assign(content.Get() + 1, content.GetEnd() - 1);
		}
	};

	template<>
	struct PropertySerializer<Vector2, E_SERIALIZER_TEXT>
	{
		static void Save(IOStream* stream, const void * mem)
		{
			const Vector2* vec2 = (const Vector2*)mem;

			AStackString<> content;
			content.Format("(%.2f, %.2f)", vec2->x, vec2->y);
			stream->Write(content.Get(), content.GetLength());
			stream->Write("\n", 1);
		}
		static void Load(const IOStream* stream, void * mem)
		{
			stream->Read(nullptr, 1);	// (

			AString contentX, contentY;
			stream->ReadLine(contentX, ',');
			stream->ReadLine(contentY, ')');
			stream->Read(nullptr, 1);

			Vector2* vec2 = (Vector2*)mem;
			sscanf_s(contentX.Get(), "%f", &(vec2->x));
			sscanf_s(contentY.Get(), "%f", &(vec2->y));
		}
	};
}

//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_SERIALIZERPROPERTY_TEXT_H