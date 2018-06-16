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
#include "Foundation/String/StringHelper.h"
#include "Foundation/Template/Macros.h"

#include <stdarg.h>
#include <stdio.h>


// String
//------------------------------------------------------------------------------
template<typename CharType, typename Allocator = DefaultAllocator>
class String : public Allocator
{
public:
	/*explicit*/ String();
	/*explicit*/ String(const CharType * string);
	/*explicit*/ String(const CharType * start, const CharType * end);

	template<typename OtherAllocator>
	String(const String<CharType, OtherAllocator> & string);
	String(const String & string);	
	~String();

	FORCE_INLINE SIZET GetLength() const { return m_Length; }
	FORCE_INLINE bool  IsEmpty() const { return (m_Length == 0); }

	// C-style compatibility
	FORCE_INLINE CharType *	      Get() { return m_Contents; }
	FORCE_INLINE const CharType * Get() const { return m_Contents; }
	FORCE_INLINE CharType *       GetEnd() { return (m_Contents + m_Length); }
	FORCE_INLINE const CharType * GetEnd() const { return (m_Contents + m_Length); }
	FORCE_INLINE CharType &	      operator [] (SIZET index) { ASSERT(index < m_Length); return m_Contents[index]; }
	FORCE_INLINE const CharType & operator [] (SIZET index)  const { ASSERT(index < m_Length); return m_Contents[index]; }

	// A pre-constructed global empty string for convenience
	static const String &   GetEmpty() { return s_EmptyStr; }
	static const CharType * GetEmptyCStr() { return s_EmptyCStr; }

	// Assignment
	void Assign(const CharType * string);
	void Assign(const CharType * start, const CharType * end);
	template<typename OtherAllocator>
	void Assign(const String<CharType, OtherAllocator> & string);
	void Clear();
	void SetCapacity(SIZET capacity);
	void SetLength(SIZET length);		// NOTE: be careful to use this function
	FORCE_INLINE SIZET GetCapacity() const { return m_Capacity; }

	// Overwrite default operator = !!!!
	FORCE_INLINE String & operator = (const CharType * string) { Assign(string); return *this; }
	FORCE_INLINE String & operator = (const String & string) { Assign(string); return *this; }
	template<typename OtherAllocator>
	FORCE_INLINE String & operator = (const String<CharType, OtherAllocator> & string) { Assign(string); return *this; }

	// C++ 11 [new]
	template<class = std::enable_if<std::is_move_constructible<Allocator>::value>::type>
	String(String && string);
	template<class = std::enable_if<std::is_move_assignable<Allocator>::value>::type>
	String & operator = (String && string);

	// Concatenation
	String<CharType, Allocator> & operator += (CharType c);
	String<CharType, Allocator> & operator += (const CharType * string);
	template<typename OtherAllocator>
	String<CharType, Allocator> & operator += (const String<CharType, OtherAllocator> & string);
	void Append(const CharType * string, SIZET len);
	void AppendFormat(const CharType * fmtString, ...);
	void AppendMultiply(const CharType * string, SIZET count);
	template<typename OtherAllocator>
	FORCE_INLINE void Append(const String<CharType, OtherAllocator> & string) { this->operator += (string); }

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
	SIZET Replace(CharType from, CharType to, SIZET maxReplaces = 0);
	SIZET Replace(const CharType * from, const CharType * to, SIZET maxReplaces = 0);
	FORCE_INLINE void ToLower() { return StringHelper::ToLower(m_Contents, m_Length); }
	FORCE_INLINE void ToUpper() { return StringHelper::ToUpper(m_Contents, m_Length); }
	
	// Searching
	CharType *		 Find(CharType c, CharType * startPos = nullptr) { return const_cast<CharType*>(StringHelper::Find(m_Contents, m_Length, c, startPos)); }
	const CharType * Find(CharType c, const CharType * startPos = nullptr) const { return StringHelper::Find(m_Contents, m_Length, c, startPos); }
	CharType *		 Find(const CharType * subString) { return const_cast<CharType*>(StringHelper::Find(m_Contents, m_Length, subString)); }
	const CharType * Find(const CharType * subString) const { return StringHelper::Find(m_Contents, m_Length, subString); }
	
	const CharType * FindI(const CharType * subString) const { return StringHelper::FindI(m_Contents, m_Length, subString); }
	const CharType * FindLast(CharType c) const { return StringHelper::FindLast(m_Contents, m_Length, c); }
	CharType *		 FindLast(CharType c) { return const_cast<CharType*>(((const String*)this)->FindLast(c)); }

	FORCE_INLINE bool EndsWith(CharType c) const { return StringHelper::EndsWith(m_Contents, m_Length, c); }
	FORCE_INLINE bool EndsWith(const CharType * string) const { return StringHelper::EndsWith(m_Contents, m_Length, string); }
	template<typename OtherAllocator>
	FORCE_INLINE bool EndsWith(const String<CharType, OtherAllocator> & other) const { return StringHelper::EndsWith(m_Contents, m_Length, other.Get()); }
	FORCE_INLINE bool EndsWithI(const CharType * other) const { return StringHelper::EndsWithI(m_Contents, m_Length, other); }
	template<typename OtherAllocator>
	FORCE_INLINE bool EndsWithI(const String<CharType, OtherAllocator> & other) const { return StringHelper::EndsWithI(m_Contents, m_Length, other.Get()); }
	FORCE_INLINE bool BeginsWith(CharType c) const { return StringHelper::BeginsWith(m_Contents, c); }
	FORCE_INLINE bool BeginsWith(const CharType * string) const { return StringHelper::BeginsWith(m_Contents, string); }
	template<typename OtherAllocator>
	FORCE_INLINE bool BeginsWith(const String<CharType, OtherAllocator> & string) const { return StringHelper::BeginsWith(m_Contents, string.Get()); }
	FORCE_INLINE bool BeginsWithI(const CharType * string) const { return StringHelper::BeginsWithI(m_Contents, string); }
	template<typename OtherAllocator>
	FORCE_INLINE bool BeginsWithI(const String<CharType, OtherAllocator> & string) const { return StringHelper::BeginsWithI(m_Contents, string.Get()); }
	
	// Pattern matching
	FORCE_INLINE static bool Match(const CharType * pattern, const CharType * string) { return StringHelper::Match(pattern, string); }
	FORCE_INLINE static bool MatchI(const CharType * pattern, const CharType * string) { return StringHelper::MatchI(pattern, string); }
	FORCE_INLINE bool Matches(const CharType * pattern) const { return Match(pattern, m_Contents); }
	FORCE_INLINE bool MatchesI(const CharType * pattern) const { return MatchI(pattern, m_Contents); }
	
	// String manipulation
	template<typename OtherAllocator>
	FORCE_INLINE void Tokenize(Array<String<CharType, OtherAllocator>> & tokens, CharType splitChar = ' ') const { return StringHelper::Tokenize(m_Contents, m_Length, tokens, splitChar); }

	template<typename OtherAllocator>
	FORCE_INLINE int32 CompareI(const String<CharType, OtherAllocator> & other) const { return StringHelper::CompareI(m_Contents, other.Get()); }

	template<class = std::enable_if<std::is_same<CharType, CHAR>::value>::type>
	FORCE_INLINE void Format(const CharType * fmtString, ...) { va_list args; va_start(args, fmtString); VFormat(fmtString, args); va_end(args); }
	FORCE_INLINE void VFormat(const CharType * fmtString, va_list arg) { return StringHelper::VFormat(*this, fmtString, arg); }

	FORCE_INLINE static SIZET StrLen(const CharType * src) { return StringHelper::StrLen(src); }
	FORCE_INLINE static void Copy(const CharType * src, CharType * dst, SIZET len) { return StringHelper::Copy(src, dst, len); }
	FORCE_INLINE static uint32 StrNCmp(const CharType * a, const CharType * b, SIZET num) { return StringHelper::StrNCmp(a, b, num); }
	FORCE_INLINE static uint32 StrNCmpI(const CharType * a, const CharType * b, SIZET num) { return StringHelper::StrNCmpI(a, b, num); }

protected:
	void Grow(SIZET addedSize = 0);
	void SetCapacity(SIZET capacity, bool needCopy);
	void Swap(String& other);

	CharType * Allocate(SIZET size);
	CharType * Reallocate(SIZET size);
	void       Deallocate();

	CharType *  m_Contents;			// Always points to valid null terminated string (even when empty)
	SIZET		m_Length;			// Length in CharTypeacters
	SIZET		m_Capacity;

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

template<int SIZE = 256, bool SUPPORT_OVERFLOW = true>
using AStackString = String<CHAR, StackAllocator<SIZE, SUPPORT_OVERFLOW>>;
template<int SIZE = 256, bool SUPPORT_OVERFLOW = true>
using WStackString = String<WCHAR, StackAllocator<SIZE, SUPPORT_OVERFLOW>>;


// Inl includes
//-----------------------------------------------------------------------------
#include "String.inl"

//------------------------------------------------------------------------------
#endif // FOUNDATION_STRING_STRING_H