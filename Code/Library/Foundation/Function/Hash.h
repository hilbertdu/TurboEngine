// HashFunction.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_FUNCTION_HASH_H
#define FOUNDATION_FUNCTION_HASH_H

// Include
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"


// Integer hash
template<class T>
class Hash
{
public:
	size_t operator()(const T& rKey) const
	{
		return static_cast<size_t>(rKey);
	}
};

// Pointer hash
template<class T>
class Hash<T*>
{
public:
	size_t operator()(const T* pKey) const
	{
		return reinterpret_cast<UINTPTR>(pKey);
	}
};

// Pointer hash
template<class T>
class Hash<const T*>
{
public:
	size_t operator()(const T* pKey) const
	{
		return reinterpret_cast<UINTPTR>(pKey);
	}
};


#endif // FOUNDATION_FUNCTION_HASH_H