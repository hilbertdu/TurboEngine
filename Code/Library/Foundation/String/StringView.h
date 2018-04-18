// StringView.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_STRING_VIEW_H
#define FOUNDATION_STRING_VIEW_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/String/String.h"


#define STRING_VIEW_CHOOSE_TYPE(strtype, contents, func, ...)	\
	switch (strtype)											\
	{															\
	case E_TYPE_STRING_HEAP:																							\
		reinterpret_cast<StringTypeTraits<CharType, E_TYPE_STRING_HEAP>::Type*>(contents)->func(__VA_ARGS__);			\
		break;																											\
	case E_TYPE_STRING_STACK:																							\
		reinterpret_cast<StringTypeTraits<CharType, E_TYPE_STRING_STACK>::Type*>(contents)->func(__VA_ARGS__);			\
		break;																											\
	default:																											\
		ASSERT(false, "Not a valid string view type 1!");																\
	}

#define STRING_VIEW_CHOOSE_TYPE_RET(strtype, contents, func, ...)	\
	switch (strtype)												\
	{																\
	case E_TYPE_STRING_HEAP:																							\
		return reinterpret_cast<StringTypeTraits<CharType, E_TYPE_STRING_HEAP>::Type*>(contents)->func(__VA_ARGS__);	\
	case E_TYPE_STRING_STACK:																							\
		return reinterpret_cast<StringTypeTraits<CharType, E_TYPE_STRING_STACK>::Type*>(contents)->func(__VA_ARGS__);	\
	default:																											\
		ASSERT(false, "Not a valid string view type 2!"); abort();														\
	}



template<class CharType, int32 TYPE>
struct StringTypeTraits;

template<class CharType> struct StringTypeTraits<CharType, ALLOC_TYPE_HEAP>		{ using Type = String<CharType, HeapAllocator>; };
template<class CharType> struct StringTypeTraits<CharType, ALLOC_TYPE_STACK>	{ using Type = String<CharType, StackAllocator<256, true>>; };


// Class ConstStringView
//------------------------------------------------------------------------------
template<class CharType>
class StringView
{
public:
	StringView()
		: m_Contents(nullptr)
		, m_Length(0)
	{}

	StringView(const CharType * str)
		: m_Contents(str)
		, m_Length(strlen(str))
	{}

	StringView(const CharType * str, SIZET size)
		: m_Contents(str)
		, m_Length(size)
	{}

	StringView(const CharType * start, const CharType * end)
		: m_Contents(start)
		, m_Length(end - start)
	{}

	template<class Allocator>
	StringView(const String<CharType, Allocator> & str)
		: m_Contents(str.Get())
		, m_Length(str.GetLength())
	{}

	StringView(const StringView& other)
		: m_Contents(other.m_Contents)
		, m_Length(other.m_Length)
	{}

	~StringView() = default;

	FORCE_INLINE void  SetContent(const CharType * str) { m_Contents = str; m_Length = strlen(str); }
	FORCE_INLINE SIZET GetLength() const { return m_Length; }
	FORCE_INLINE bool  IsEmpty() const { return (m_Length == 0); }

	// C-style compatibility
	FORCE_INLINE const CharType * Get() const { return m_Contents; }
	FORCE_INLINE const CharType * GetEnd() const { return (m_Contents + m_Length); }
	FORCE_INLINE const CharType & operator [] (SIZET index)  const { ASSERT(index < m_Length); return m_Contents[index]; }

	// Comparison
	FORCE_INLINE bool operator == (const CharType * other) const { return StringHelper::Compare(m_Contents, other); }
	template<typename OtherAllocator>
	FORCE_INLINE bool operator == (const String<CharType, OtherAllocator> & other) const { return StringHelper::Compare(m_Contents, other.Get()); }
	FORCE_INLINE bool operator != (const CharType * other) const { return !(*this == other); }
	template<typename OtherAllocator>
	FORCE_INLINE bool operator != (const String<CharType, OtherAllocator> & other) const { return !(*this == other); }
	template<typename OtherAllocator>
	FORCE_INLINE bool operator < (const String<CharType, OtherAllocator> & other) const { return (StringHelper::CompareI(m_Contents, other.Get()) < 0); }

	// Searching
	FORCE_INLINE const CharType * Find(CharType c, const CharType * startPos = nullptr) const { return StringHelper::Find(m_Contents, m_Length, c, startPos); }
	FORCE_INLINE const CharType * Find(const CharType * subString) const { return StringHelper::Find(m_Contents, m_Length, subString); }

	FORCE_INLINE const CharType * FindI(const CharType * subString) const { return StringHelper::FindI(m_Contents, m_Length, subString); }
	FORCE_INLINE const CharType * FindLast(CharType c) const { return StringHelper::FindLast(m_Contents, m_Length, c); }

	FORCE_INLINE bool EndsWith(CharType c) const { return StringHelper::EndsWith(m_Contents, m_Length, c); }
	FORCE_INLINE bool EndsWith(const CharType * string) const { return StringHelper::EndsWith(m_Contents, m_Length, string); }
	template<typename Allocator>
	FORCE_INLINE bool EndsWith(const String<CharType, Allocator> & other) const { return StringHelper::EndsWith(m_Contents, m_Length, other.Get()); }
	FORCE_INLINE bool EndsWithI(const CharType * other) const { return StringHelper::EndsWithI(m_Contents, m_Length, other); }
	template<typename Allocator>
	FORCE_INLINE bool EndsWithI(const String<CharType, Allocator> & other) const { return StringHelper::EndsWithI(m_Contents, m_Length, other.Get()); }
	FORCE_INLINE bool BeginsWith(CharType c) const { return StringHelper::BeginsWith(m_Contents, c); }
	FORCE_INLINE bool BeginsWith(const CharType * string) const { return StringHelper::BeginsWith(m_Contents, string); }
	template<typename Allocator>
	FORCE_INLINE bool BeginsWith(const String<CharType, Allocator> & string) const { return StringHelper::BeginsWith(m_Contents, string.Get()); }
	FORCE_INLINE bool BeginsWithI(const CharType * string) const { return StringHelper::BeginsWithI(m_Contents, string); }
	template<typename Allocator>
	FORCE_INLINE bool BeginsWithI(const String<CharType, Allocator> & string) const { return StringHelper::BeginsWithI(m_Contents, string.Get()); }

	// Pattern matching
	FORCE_INLINE static bool Match(const CharType * pattern, const CharType * string) { return StringHelper::Match(pattern, string); }
	FORCE_INLINE static bool MatchI(const CharType * pattern, const CharType * string) { return StringHelper::MatchI(pattern, string); }
	FORCE_INLINE bool Matches(const CharType * pattern) const { return Match(pattern, m_Contents); }
	FORCE_INLINE bool MatchesI(const CharType * pattern) const { return MatchI(pattern, m_Contents); }

protected:
	const char* m_Contents;
	uint32		m_Length;
};


// Class StringView
//------------------------------------------------------------------------------
template<class CharType>
class StringRef : public StringView<CharType>
{
public:
	enum StringType : uint8
	{
		E_TYPE_STRING_HEAP = ALLOC_TYPE_HEAP,
		E_TYPE_STRING_STACK = ALLOC_TYPE_STACK
	};

	template<class Allocator>
	StringRef(String<CharType, Allocator> & str)
		: StringView(str)
		, m_StrPtr(&str)
		, m_StrType(Allocator::ALLOC_TYPE)
	{}

	template<class Allocator>
	StringRef(const String<CharType, Allocator> & str)
		: StringView(str)
		, m_StrPtr(&str)
		, m_StrType(Allocator::ALLOC_TYPE)
	{}

	StringRef(const StringRef& other)
		: m_StrPtr(other.m_Contents)
		, m_StrType(Allocator::ALLOC_TYPE)
		, m_Contents(other.m_Contents)
		, m_Length(other.m_Length)
	{}

	~StringRef() = default;

	// C-style compatibility
	FORCE_INLINE CharType & operator [] (SIZET index) { STRING_VIEW_CHOOSE_TYPE_RET(m_StrType, m_StrPtr, operator[], index); }

	FORCE_INLINE void Assign(const CharType * string) { STRING_VIEW_CHOOSE_TYPE(m_StrType, m_StrPtr, Assign, string); }
	FORCE_INLINE void Assign(const CharType * start, const CharType * end) { STRING_VIEW_CHOOSE_TYPE(m_StrType, m_StrPtr, Assign, start, end); }

	// Overwrite default operator = !!!!
	FORCE_INLINE StringRef & operator = (const char * string) { STRING_VIEW_CHOOSE_TYPE(m_StrType, m_StrPtr, operator=, string); return *this; }
	FORCE_INLINE StringRef & operator = (const StringRef & string) { STRING_VIEW_CHOOSE_TYPE(m_StrType, m_StrPtr, operator=, string); return *this; }

	// Concatenation
	FORCE_INLINE StringRef & operator += (CharType c) { STRING_VIEW_CHOOSE_TYPE(m_StrType, m_StrPtr, operator+=, c); return *this; }
	FORCE_INLINE StringRef & operator += (const CharType * string) { STRING_VIEW_CHOOSE_TYPE(m_StrType, m_StrPtr, operator+=, string); return *this; }
	FORCE_INLINE StringRef & operator += (const StringRef & string) { STRING_VIEW_CHOOSE_TYPE(m_StrType, m_StrPtr, operator+=, string.Get()); return *this; }

	// Transformations
	FORCE_INLINE SIZET Replace(CharType from, CharType to, SIZET maxReplaces = 0) { STRING_VIEW_CHOOSE_TYPE_RET(m_StrType, m_StrPtr, Replace, from, to, maxReplaces); }
	FORCE_INLINE SIZET Replace(const CharType * from, const CharType * to, SIZET maxReplaces = 0){ STRING_VIEW_CHOOSE_TYPE_RET(m_StrType, m_StrPtr, Replace, from, to, maxReplaces); }

	// ChangeSize
	FORCE_INLINE void Clear() { STRING_VIEW_CHOOSE_TYPE(m_StrType, m_StrPtr, Clear); }
	FORCE_INLINE void SetCapacity(SIZET capacity) { STRING_VIEW_CHOOSE_TYPE(m_StrType, m_StrPtr, SetCapacity, capacity); }
	FORCE_INLINE void SetLength(SIZET length) { STRING_VIEW_CHOOSE_TYPE(m_StrType, m_StrPtr, SetLength, length); }

private:
	void *	m_StrPtr;
	uint8	m_StrType;
};


// StringView alias
//------------------------------------------------------------------------------
using AStringView = StringView<CHAR>;
using WStringView = StringView<WCHAR>;
using AStringRef  = StringRef<CHAR>;
using WStringRef  = StringRef<WCHAR>;


// Operator +
//------------------------------------------------------------------------------
FORCE_INLINE AString operator+ (AStringView lhs, AStringView rhs)
{
	AString str(lhs.Get());
	str += rhs.Get();
	return std::move(str);
}


//------------------------------------------------------------------------------
#endif // FOUNDATION_STRING_VIEW_H