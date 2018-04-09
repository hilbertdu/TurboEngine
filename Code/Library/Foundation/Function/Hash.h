// HashFunction.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_FUNCTION_HASH_H
#define FOUNDATION_FUNCTION_HASH_H

// Include
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/String/String.h"
#include <type_traits>


// Simple
class Hasher_Cast
{
public:
	inline SIZET Hash(SIZET pKey) const
	{
		return pKey;
	}
};

// FNV-hash
class Hasher_FNV_1
{
public:
	inline SIZET Hash(const uint8 * pKey, SIZET count) const
	{
#if defined(__WIN64__)
		static_assert(sizeof(SIZET) == 8, "This code is for 64-bit size_t.");
		const SIZET _FNV_offset_basis = 14695981039346656037ULL;
		const SIZET _FNV_prime = 1099511628211ULL;
#else /* defined(__WIN64__) */
		static_assert(sizeof(SIZET) == 4, "This code is for 32-bit size_t.");
		const SIZET _FNV_offset_basis = 2166136261U;
		const SIZET _FNV_prime = 16777619U;
#endif /* defined(__WIN64__) */

		SIZET _Val = _FNV_offset_basis;
		for (SIZET _Next = 0; _Next < count; ++_Next)
		{	// fold in another byte
			_Val ^= (SIZET)pKey[_Next];
			_Val *= _FNV_prime;
		}
		return (_Val);
	}
};

class Hasher_FNV_2
{
public:
	inline SIZET Hash(const uint8 * pKey, SIZET count) const
	{
#if defined(__WIN64__)
		static_assert(sizeof(SIZET) == 8, "This code is for 64-bit size_t.");
		const SIZET _FNV_offset_basis = 14695981039346656037ULL;
		const SIZET _FNV_prime = 1099511628211ULL;
#else /* defined(__WIN64__) */
		static_assert(sizeof(SIZET) == 4, "This code is for 32-bit size_t.");
		const SIZET _FNV_offset_basis = 2166136261U;
		const SIZET _FNV_prime = 16777619U;
#endif /* defined(__WIN64__) */

		SIZET _Val = _FNV_offset_basis;
		for (SIZET _Next = 0; _Next < count; ++_Next)
		{	// fold in another byte
			_Val ^= (SIZET)pKey[_Next];
			_Val *= _FNV_prime;
		}
		return (_Val);
	}
};


template<class T>
class SimpleHasher : public Hasher_Cast
{
public:
	template<bool>
	struct _Hash
	{
		SIZET __Hasher(const T& rKey) { return Hash((SIZET)(&rKey)); }
	};

	template<>
	struct _Hash<true>
	{
		SIZET __Hasher(const T& rKey) { return Hash((SIZET)(rKey)); }
	};

	SIZET operator()(const T& rKey) const
	{
		return _Hash<std::is_integral<T>::value>::__Hasher(rKey);
	}
};


template<class T>
class FNVHasher : public Hasher_FNV_1
{
public:
	SIZET operator()(const T& rKey) const
	{
		return Hash((const unsigned char *)&rKey, sizeof(T));
	}
};


template<>
class FNVHasher<AString> : public Hasher_FNV_1
{
public:
	SIZET operator()(const AString& rKey) const
	{
		return Hash((const unsigned char *)rKey.Get(), rKey.GetLength());
	}
};




#endif // FOUNDATION_FUNCTION_HASH_H