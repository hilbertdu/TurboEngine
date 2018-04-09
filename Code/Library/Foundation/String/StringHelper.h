// StringHelper.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_STRING_HELPER_H
#define FOUNDATION_STRING_HELPER_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Container/Array.h"


namespace StringHelper
{
	// Transform
	template<class CharType>
	static void ToLower(CharType * src, SIZET size);
	template<class CharType>
	static void ToUpper(CharType * src, SIZET size);

	// Searching
	template<class CharType>
	static const CharType * Find(const CharType * src, SIZET size, CharType c, const CharType * startPos = nullptr);
	template<class CharType>
	static const CharType * Find(const CharType * src, SIZET size, const CharType * subString);
	template<class CharType>
	static const CharType * FindI(const CharType * src, SIZET size, const CharType * subString);
	template<class CharType>
	static const CharType * FindLast(const CharType * src, SIZET size, CharType c);

	template<class CharType>
	static bool EndsWith(const CharType * src, SIZET size, CharType c);
	template<class CharType>
	static bool EndsWith(const CharType * src, SIZET size, const CharType * string);
	template<class CharType>
	static bool EndsWithI(const CharType * src, SIZET size, const CharType * other);
	template<class CharType>
	static bool BeginsWith(const CharType * src, CharType c);
	template<class CharType>
	static bool BeginsWith(const CharType * src, const CharType * string);
	template<class CharType>
	static bool BeginsWithI(const CharType * src, const CharType * string);

	// Pattern matching
	template<class CharType>
	static bool Match(const CharType * pattern, const CharType * string);
	template<class CharType>
	static bool MatchI(const CharType * pattern, const CharType * string);
	template<class CharType>
	static bool Matches(const CharType * src, SIZET size, const CharType * pattern);
	template<class CharType>
	static bool MatchesI(const CharType * src, SIZET size, const CharType * pattern);

	// String manipulation helpers
	template<class CharType>
	static void Copy(const CharType * src, CharType * dst, SIZET len);
	template<class CharType>
	static SIZET StrLen(const CharType * src);
	template<class CharType>
	static uint32 StrNCmp(const CharType * a, const CharType * b, SIZET num);
	template<class CharType>
	static uint32 StrNCmpI(const CharType * a, const CharType * b, SIZET num);

	// Compare
	template<class CharType>
	static bool Compare(const CharType * src, const CharType * des);
	static int32 CompareI(const CHAR * src, const CHAR * des);
	static int32 CompareI(const WCHAR * src, const WCHAR * des);

	// Tokenize
	template<class CharType, class StringType>
	static void Tokenize(const CharType * src, SIZET size, Array<StringType, HeapAllocator> & tokens, CharType splitChar = ' ');

	// Format
	template<class StringType>
	static void Format(StringType & des, const CHAR * fmtString, ...);
	template<class StringType>
	static void Format(StringType & des, const WCHAR * fmtString, ...);
	template<class StringType>
	static void VFormat(StringType & des, const CHAR * fmtString, va_list arg);
	template<class StringType>
	static void VFormat(StringType & des, const WCHAR * fmtString, va_list arg);
}

#include "Foundation/String/StringHelper.inl"


//------------------------------------------------------------------------------
#endif // FOUNDATION_STRING_HELPER_H