// Iterator.h
// container iterators
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_ITERATOR_H
#define FOUNDATION_REFLECTION_ITERATOR_H


// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"

namespace TReflection
{
	struct IReadIterator
	{
		virtual ~IReadIterator() {}
		virtual IReadIterator* Clone() = 0;
		virtual const void* GetKey() const = 0;
		virtual const void* GetValue() const = 0;
		virtual uint32 GetCount() const = 0;
		virtual void MoveNext() = 0;
		virtual bool IsValid() const = 0;		
		virtual void ResetContainer(const void *container) = 0;
		virtual bool IsKeyPointer() const = 0;
		virtual bool IsValuePointer() const = 0;		
	};

	struct IWriteIterator
	{
		virtual ~IWriteIterator() {}
		virtual IWriteIterator* Clone() = 0;
		virtual void Add(void* object) = 0;
		virtual void Add(void* key, void* object) = 0;
		virtual void* AddEmpty() = 0;
		virtual void* AddEmpty(void* key) = 0;
		virtual void ResetContainer(const void *container) = 0;
		virtual bool IsKeyPointer() const = 0;
		virtual bool IsValuePointer() const = 0;
	};
}

#endif // FOUNDATION_REFLECTION_ITERATOR_H
//------------------------------------------------------------------------------
