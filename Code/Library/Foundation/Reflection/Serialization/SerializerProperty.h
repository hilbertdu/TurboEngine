// SerializerText.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_SERIALIZERPROPERTY_H
#define FOUNDATION_REFLECTION_SERIALIZERPROPERTY_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/Serialization/Serializer.h"

namespace TReflection
{
	template<class T, uint8 STYPE>
	struct PropertySerializer;

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
		}
		static void Load(const IOStream* stream, void * mem)
		{
			if (AString::StrNCmp((const char*)stream->GetCurData(), "true", 4) == 0)
			{
				*(bool*)mem = true;
				stream->Read(nullptr, 4);
			}
			else if (AString::StrNCmp((const char*)stream->GetCurData(), "false", 5) == 0)
			{
				*(bool*)mem = false;
				stream->Read(nullptr, 5);
			}
		}
	};

	template<>
	struct PropertySerializer<float, E_SERIALIZER_TEXT>
	{
		static void Save(IOStream* stream, const void * mem)
		{
			AString content;
			content.Format("%1.1f", *(float*)mem);
			stream->Write(content.Get(), content.GetLength());
		}
		static void Load(const IOStream* stream, void * mem)
		{
			//stream->Read(nullptr, sscanf_s((const char*)stream->GetCurData(), "%f", (float*)mem));
			STREAM_READ_SCANF(stream, %f, (float*)mem);
		}
	};

	template<>
	struct PropertySerializer<double, E_SERIALIZER_TEXT>
	{
		static void Save(IOStream* stream, const void * mem)
		{
			AString content;
			content.Format("%1.1lf", *(double*)mem);
			stream->Write(content.Get(), content.GetLength());
		}
		static void Load(const IOStream* stream, void * mem)
		{
			//stream->Read(nullptr, sscanf_s((const char*)stream->GetCurData(), "%lf", (double*)mem));
			STREAM_READ_SCANF(stream, %lf, (double*)mem);
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
		}
		static void Load(const IOStream* stream, void * mem)
		{
			uint32 tmp;
			//stream->Read(nullptr, sscanf_s((const char*)stream->GetCurData(), "%u", &tmp));
			STREAM_READ_SCANF(stream, %u, &tmp);
			ASSERT(tmp <= 255);
			*(uint8*)mem = (uint8)tmp;
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
		}
		static void Load(const IOStream* stream, void * mem)
		{
			uint32 tmp;
			//stream->Read(nullptr, sscanf_s((const char*)stream->GetCurData(), "%u", &tmp));
			STREAM_READ_SCANF(stream, %u, &tmp);
			ASSERT(tmp <= 65536);
			*(uint16*)mem = (uint16)tmp;
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
		}
		static void Load(const IOStream* stream, void * mem)
		{
			//stream->Read(nullptr, sscanf_s((const char*)stream->GetCurData(), "%u", (uint32*)mem));
			STREAM_READ_SCANF(stream, %u, (uint32*)mem);
		}
	};

	template<>
	struct PropertySerializer<uint64, E_SERIALIZER_TEXT>
	{
		static void Save(IOStream* stream, const void * mem)
		{
			AString content;
			content.Format("%llu", *(uint64*)mem);
			stream->Write(content.Get(), content.GetLength());
		}
		static void Load(const IOStream* stream, void * mem)
		{
			//stream->Read(nullptr, sscanf_s((const char*)stream->GetCurData(), "%llu", (uint64*)mem));
			STREAM_READ_SCANF(stream, %llu, (uint64*)mem);
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
		}
		static void Load(const IOStream* stream, void * mem) 
		{
			int32 tmp;
			//stream->Read(nullptr, sscanf_s((const char*)stream->GetCurData(), "%i", &tmp));
			STREAM_READ_SCANF(stream, %i, &tmp);
			ASSERT(tmp >= -128 && tmp <= 127);
			*(int8*)mem = (int8)tmp;
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
		}
		static void Load(const IOStream* stream, void * mem) 
		{
			int32 tmp;
			//stream->Read(nullptr, sscanf_s((const char*)stream->GetCurData(), "%i", &tmp));
			STREAM_READ_SCANF(stream, %i, &tmp);
			ASSERT(tmp >= -32768 && tmp <= 32767);
			*(int16*)mem = (int16)tmp;
		}
	};

	template<>
	struct PropertySerializer<int32, E_SERIALIZER_TEXT>
	{
		static void Save(IOStream* stream, const void * mem)
		{
			AString content;
			content.Format("%i", *(int32*)mem);
			stream->Write(content.Get(), content.GetLength());;
		}
		static void Load(const IOStream* stream, void * mem) 
		{
			//stream->Read(nullptr, sscanf_s((const char*)stream->GetCurData(), "%i", (int32*)mem));
			STREAM_READ_SCANF(stream, %i, (int32*)mem);
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
		}
		static void Load(const IOStream* stream, void * mem)
		{
			//stream->Read(nullptr, sscanf_s((const char*)stream->GetCurData(), "%lld", (int64*)mem));
			STREAM_READ_SCANF(stream, %lld, (int64*)mem);
		}
	};

	template<>
	struct PropertySerializer<AString, E_SERIALIZER_TEXT>
	{
		static void Save(IOStream* stream, const void * mem)
		{
			const AString* str = (const AString*)mem;
			const uint64 length = str->GetLength();
			PropertySerializer<uint64, E_SERIALIZER_TEXT>::Save(stream, &length);
			stream->Write(str->Get(), str->GetLength());
		}
		static void Load(const IOStream* stream, void * mem) 
		{
			AString* str = (AString*)mem;
			uint32 length;
			PropertySerializer<uint32, E_SERIALIZER_TEXT>::Load(stream, (void*)&length);
			str->SetLength(length);
			stream->Read(str->Get(), length);
		}
	};

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
}


//------------------------------------------------------------------------------
#endif // FOUNDATION_REFLECTION_SERIALIZERPROPERTY_H