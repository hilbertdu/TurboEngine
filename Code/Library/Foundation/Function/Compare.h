// HashFunction.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_FUNCTION_COMPARE_H
#define FOUNDATION_FUNCTION_COMPARE_H

// Include
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"


// Equal
//------------------------------------------------------------------------------
template<typename T>
class Equals
{
public:
	bool operator()(const T& rA, const T& rB) const
	{
		return rA == rB;
	}
};

// Equal ansi string
//------------------------------------------------------------------------------
template<>
class Equals<char*>
{
public:
	bool operator()(const char* pA, const char* pB) const
	{
	}
};

template<>
class Equals<const char*>
{
public:
	bool operator()(const char* pA, const char* pB) const
	{
	}
};

// Equal wide string
//------------------------------------------------------------------------------
template<>
class Equals<wchar_t*>
{
public:
	bool operator()(const wchar_t* pA, const wchar_t* pB) const
	{
	}
};

template<>
class Equals<const wchar_t*>
{
public:
	bool operator()(const wchar_t* pA, const wchar_t* pB) const
	{
	}
};

// Less than
//------------------------------------------------------------------------------
template<typename T>
class Less
{
public:
	bool operator()(const T& rA, const T& rB) const
	{
		return rA < rB;
	}
};

// Less than string
//------------------------------------------------------------------------------
template<>
class Less<char*>
{
public:
	bool operator()(const char* pA, const char* pB) const
	{
	}
};

template<>
class Less<const char*>
{
public:
	bool operator()(const char* pA, const char* pB) const
	{
	}
};

// Less than wide string
//------------------------------------------------------------------------------
template<>
class Less<wchar_t*>
{
public:
	bool operator()(const wchar_t* pA, const wchar_t* pB) const
	{
	}
};

template<>
class Less<const wchar_t*>
{
public:
	bool operator()(const wchar_t* pA, const wchar_t* pB) const
	{
	}
};

// Identity
//------------------------------------------------------------------------------
template<typename T>
class Identity
{
public:
	T& operator()(T& rValue) const
	{
		return rValue;
	}
	const T& operator()(const T& rValue) const
	{
		return rValue;
	}
};

template<typename T>
class Identity<const T>
{
public:
	const T& operator()(const T& rValue) const
	{
		return rValue;
	}
};


#endif // FOUNDATION_FUNCTION_COMPARE_H
