// Serializer.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_SERIALIZER_H
#define FOUNDATION_REFLECTION_SERIALIZER_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/FileIO/IOStream.h"

namespace TReflection
{
	class IMetaType;

	void Save(IOStream & stream, const void * object, const IMetaType * objectType);
	void Load(const IOStream & stream, void * object, const IMetaType * objectType);

	class ISerializer
	{
	public:
		virtual ~ISerializer() {}
		virtual void Save(IOStream & stream, const void * mem) = 0;
		virtual void Load(const IOStream & stream, void * mem) = 0;
	};

	template<class T>
	class Serializer;

	template<>
	class Serializer<bool> : public ISerializer
	{
		virtual void Save(IOStream & stream, const void * mem) {}
		virtual void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	class Serializer<float> : public ISerializer
	{
		virtual void Save(IOStream & stream, const void * mem) {}
		virtual void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	class Serializer<double> : public ISerializer
	{
		virtual void Save(IOStream & stream, const void * mem) {}
		virtual void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	class Serializer<uint8> : public ISerializer
	{
		virtual void Save(IOStream & stream, const void * mem) {}
		virtual void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	class Serializer<uint16> : public ISerializer
	{
		virtual void Save(IOStream & stream, const void * mem) {}
		virtual void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	class Serializer<uint32> : public ISerializer
	{
		virtual void Save(IOStream & stream, const void * mem) {}
		virtual void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	class Serializer<uint64> : public ISerializer
	{
		virtual void Save(IOStream & stream, const void * mem) {}
		virtual void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	class Serializer<int8> : public ISerializer
	{
		virtual void Save(IOStream & stream, const void * mem) {}
		virtual void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	class Serializer<int16> : public ISerializer
	{
		virtual void Save(IOStream & stream, const void * mem) {}
		virtual void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	class Serializer<int32> : public ISerializer
	{
		virtual void Save(IOStream & stream, const void * mem) {}
		virtual void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	class Serializer<int64> : public ISerializer
	{
		virtual void Save(IOStream & stream, const void * mem) {}
		virtual void Load(const IOStream & stream, void * mem) {}
	};

	template<>
	class Serializer<AString> : public ISerializer
	{
		virtual void Save(IOStream & stream, const void * mem) {}
		virtual void Load(const IOStream & stream, void * mem) {}
	};
}


#endif // FOUNDATION_REFLECTION_SERIALIZER_H

