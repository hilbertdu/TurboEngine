// String.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_STRING_STRING_H
#define FOUNDATION_STRING_STRING_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Container/Array.h"
#include "Foundation/Env/Assert.h"
#include "Foundation/Platform/Types.h"
#include "Foundation/Memory/Allocator.h"
#include "Foundation/Memory/Mem.h"
#include "Foundation/Math/Conversion.h"

#include <stdarg.h>
#include <stdio.h>


// String
//------------------------------------------------------------------------------
template<typename CharType, typename Allocator = DefaultAllocator>
class String
{
public:
	explicit String();
	explicit String(const CharType * string);
	explicit String(const CharType * start, const CharType * end);

	String(const String & string);

	template<class = typename std::enable_if<std::is_move_assignable<Allocator>::value>::type>
	String(String && string);

	template<typename OtherAllocator>
	String(const String<CharType, OtherAllocator> & string);
	~String();

	FORCE_INLINE size_t GetLength() const { return m_Length; }
	FORCE_INLINE bool   IsEmpty() const { return (m_Length == 0); }

	// C-style compatibility
	FORCE_INLINE CharType *	      Get() { return m_Contents; }
	FORCE_INLINE const CharType * Get() const { return m_Contents; }
	FORCE_INLINE CharType *       GetEnd() { return (m_Contents + m_Length); }
	FORCE_INLINE const CharType * GetEnd() const { return (m_Contents + m_Length); }
	FORCE_INLINE CharType &	      operator [] (size_t index) { ASSERT(index < m_Length); return m_Contents[index]; }
	FORCE_INLINE const CharType & operator [] (size_t index)  const { ASSERT(index < m_Length); return m_Contents[index]; }

	// A pre-constructed global empty string for convenience
	static const String &   GetEmpty() { return s_EmptyStr; }
	static const CharType * GetEmptyCStr() { return s_EmptyCStr; }

	// Assignment
	void Assign(const CharType * string);
	void Assign(const CharType * start, const CharType * end);
	template<typename OtherAllocator>
	void Assign(const String<CharType, OtherAllocator> & string);
	void Clear();
	void SetCapacity(size_t capacity);
	FORCE_INLINE size_t GetCapacity() const { return m_Capacity; }

	// Overwrite default operator = !!!!
	String & operator = (const CharType * string) { Assign(string); return *this; }
	String & operator = (const String & string) { Assign(string); return *this; }
	template<typename OtherAllocator>
	String & operator = (const String<CharType, OtherAllocator> & string) { Assign(string); return *this; }

	// C++ 11 [new]
	template<class = typename std::enable_if<std::is_move_assignable<Allocator>::value>::type>
	String & operator = (String && string);

	// Concatenation
	String<CharType, Allocator> & operator += (CharType c);
	String<CharType, Allocator> & operator += (const CharType * string);
	template<typename OtherAllocator>
	String<CharType, Allocator> & operator += (const String<CharType, OtherAllocator> & string);
	template<typename OtherAllocator>
	FORCE_INLINE void Append(const String<CharType, OtherAllocator> & string) { this->operator += (string); }
	void Append(const CharType * string, size_t len);
	void AppendFormat(const CharType * fmtString, ...);

	// Comparison
	bool operator == (const CharType * other) const;
	template<typename OtherAllocator>
	bool operator == (const String<CharType, OtherAllocator> & other) const;
	FORCE_INLINE bool operator != (const CharType * other) const { return !(*this == other); }
	template<typename OtherAllocator>
	FORCE_INLINE bool operator != (const String<CharType, OtherAllocator> & other) const { return !(*this == other); }
	template<typename OtherAllocator>
	FORCE_INLINE bool operator < (const String<CharType, OtherAllocator> & other) const { return (CompareI(other) < 0); }

	// Transformations
	size_t Replace(CharType from, CharType to, size_t maxReplaces = 0);
	size_t Replace(const CharType * from, const CharType * to, size_t maxReplaces = 0);
	void ToLower();
	void ToUpper();

	// Searching
	const CharType * Find(CharType c, const CharType * startPos = nullptr) const;
	CharType *		 Find(CharType c, CharType * startPos = nullptr) { return const_cast<CharType *>(((const String *)this)->Find(c, startPos)); }
	const CharType * Find(const CharType * subString) const;
	CharType *		 Find(const CharType * subString) { return const_cast<CharType *>(((const String *)this)->Find(subString)); }
	const CharType * FindI(const CharType * subString) const;
	const CharType * FindLast(CharType c) const;
	CharType *		 FindLast(CharType c) { return const_cast<CharType *>(((const String *)this)->FindLast(c)); }
	bool EndsWith(CharType c) const;
	bool EndsWith(const CharType * string) const;
	template<typename OtherAllocator>
	bool EndsWith(const String<CharType, OtherAllocator> & other) const;
	bool EndsWithI(const CharType * other) const;
	template<typename OtherAllocator>
	bool EndsWithI(const String<CharType, OtherAllocator> & other) const;
	bool BeginsWith(CharType c) const;
	bool BeginsWith(const CharType * string) const;
	template<typename OtherAllocator>
	bool BeginsWith(const String<CharType, OtherAllocator> & string) const;
	bool BeginsWithI(const CharType * string) const;
	template<typename OtherAllocator>
	bool BeginsWithI(const String<CharType, OtherAllocator> & string) const;

	// Pattern matching
	static bool Match(const CharType * pattern, const CharType * string);
	static bool MatchI(const CharType * pattern, const CharType * string);
	FORCE_INLINE bool Matches(const CharType * pattern) const { return Match(pattern, m_Contents); }
	FORCE_INLINE bool MatchesI(const CharType * pattern) const { return MatchI(pattern, m_Contents); }

	// String manipulation
	template<typename OtherAllocator>
	void Tokenize(Array<String<CharType, OtherAllocator>> & tokens, CharType splitCharType = ' ') const;

	// String manipulation helpers
	static void Copy(const CharType * src, CharType * dst, size_t len);
	static size_t StrLen(const CharType * string);
	static uint32 StrNCmp(const CharType * a, const CharType * b, size_t num);
	static uint32 StrNCmpI(const CharType * a, const CharType * b, size_t num);

public:
	template<typename OtherAllocator, class = typename std::enable_if<std::is_same<CharType, CHAR>::value>::type>
	int32 CompareI(const String<CHAR, OtherAllocator> & other) const;
	template<typename OtherAllocator, class = typename std::enable_if<std::is_same<CharType, WCHAR>::value>::type>
	int32 CompareI(const String<WCHAR, OtherAllocator> & other) const;

	template<class = typename std::enable_if<std::is_same<CharType, CHAR>::value>::type>
	void Format(const CHAR * fmtString, ...);
	template<class = typename std::enable_if<std::is_same<CharType, WCHAR>::value>::type>
	void Format(const WCHAR * fmtString, ...);

	template<class = typename std::enable_if<std::is_same<CharType, CHAR>::value>::type>
	void VFormat(const CHAR * fmtString, va_list arg);
	template<class = typename std::enable_if<std::is_same<CharType, WCHAR>::value>::type>
	void VFormat(const WCHAR * fmtString, va_list arg);


protected:
	void Grow(size_t addedSize = 0);
	void SetCapacity(SIZET capacity, bool needCopy);

	CharType * Allocate(size_t size);
	CharType * Reallocate(size_t size);
	void       Deallocate();

	CharType * m_Contents;			// Always points to valid null terminated string (even when empty)
	size_t     m_Length;			// Length in CharTypeacters
	size_t     m_Capacity;
	Allocator  m_AllocatorInst;

	template<class T>
	inline static const T * GetEmptyCStr() { return ""; };

	template<>
	inline static const WCHAR * GetEmptyCStr<WCHAR>() { return L""; };

	static const CharType *					 s_EmptyCStr;
	static const String<CharType, Allocator> s_EmptyStr;
};


// Static
//------------------------------------------------------------------------------
template<class CharType, class Allocator>
/*static*/ const CharType * String<CharType, Allocator>::s_EmptyCStr(GetEmptyCStr<CharType>());

template<class CharType, class Allocator>
/*static*/ const String<CharType, Allocator> String<CharType, Allocator>::s_EmptyStr(GetEmptyCStr<CharType>());

// CString alias
//------------------------------------------------------------------------------
typedef String<CHAR> AString;
typedef String<WCHAR> WString;

template<int SIZE = 1024, bool SUPPORT_OVERFLOW = true>
using AStackString = String<CHAR, StackAllocator<SIZE, SUPPORT_OVERFLOW>>;
template<int SIZE = 1024, bool SUPPORT_OVERFLOW = true>
using WStackString = String<CHAR, StackAllocator<SIZE, SUPPORT_OVERFLOW>>;


// Inl includes
//-----------------------------------------------------------------------------
#include "String.inl"
#include "StringAnsi.inl"
#include "StringWide.inl"

//------------------------------------------------------------------------------
#endif // FOUNDATION_STRING_STRING_H