// String.inl
//------------------------------------------------------------------------------


// Constructor
//------------------------------------------------------------------------------
template<typename CharType, typename Allocator>
String<CharType, Allocator>::String()
	: m_Contents(const_cast<CharType *>(GetEmptyCStr<CharType>()))
	, m_Length(0)
	, m_Capacity(0)
	, m_MemShared(true)
{
}

template<typename CharType, typename Allocator>
String<CharType, Allocator>::String(const CharType * string)
	: m_Contents(const_cast<CharType *>(GetEmptyCStr<CharType>()))
	, m_Capacity(0)
	, m_MemShared(true)
{
	ASSERT(string);
	m_Length = StrLen(string);
	SetCapacity(m_Length);
	Copy(string, m_Contents, m_Length); // copy handles terminator
}

template<typename CharType, typename Allocator>
String<CharType, Allocator>::String(const CharType * start, const CharType * end)
	: m_Contents(const_cast<CharType *>(GetEmptyCStr<CharType>()))
	, m_Capacity(0)
	, m_MemShared(true)
{
	ASSERT(start && end);
	m_Length = (size_t)(end - start);
	SetCapacity(m_Length);
	Copy(start, m_Contents, m_Length); // copy handles terminator
}

// Copy Constructor
//------------------------------------------------------------------------------
template<typename CharType, typename Allocator>
String<CharType, Allocator>::String(const String & string)
	: m_Contents(const_cast<CharType *>(GetEmptyCStr<CharType>()))
	, m_Capacity(0)
	, m_MemShared(true)
{
	m_Length = string.GetLength();
	SetCapacity(m_Length);
	Copy(string.Get(), m_Contents, m_Length); // copy handles terminator
}

template<typename CharType, typename Allocator>
template<typename OtherAllocator>
String<CharType, Allocator>::String(const String<CharType, OtherAllocator> & string)
	: m_Contents(const_cast<CharType *>(GetEmptyCStr<CharType>()))
	, m_Capacity(0)
	, m_MemShared(true)
{
	m_Length = string.GetLength();
	SetCapacity(m_Length);
	Copy(string.Get(), m_Contents, m_Length); // copy handles terminator
}

// Destructor
//------------------------------------------------------------------------------
template<typename CharType, typename Allocator>
String<CharType, Allocator>::~String()
{
	Deallocate();
}

// Operators
//------------------------------------------------------------------------------
template<typename CharType, typename Allocator>
bool String<CharType, Allocator>::operator == (const CharType * other) const
{
	const CharType * thisPos = m_Contents;
	const CharType * otherPos = other;

	while (true)
	{
		CharType c1 = *thisPos;
		CharType c2 = *otherPos;
		if (c1 != c2)
		{
			return false;
		}
		if (c1 == (CharType)'\000')
		{
			return true;
		}
		thisPos++;
		otherPos++;
	}
}

template<typename CharType, typename Allocator>
template<typename OtherAllocator>
bool String<CharType, Allocator>::operator == (const String<CharType, OtherAllocator> & other) const
{
	if (other.GetLength() != GetLength())
	{
		return false;
	}
	return (*this == other.Get());
}

template<typename CharType, typename Allocator>
void String<CharType, Allocator>::Assign(const CharType * string)
{
	Assign(string, string + StrLen(string));
}

template<typename CharType, typename Allocator>
void String<CharType, Allocator>::Assign(const CharType * start, const CharType * end)
{
	ASSERT(start && end && start <= end);
	m_Length = (size_t)(end - start);
	if (m_Length > GetCapacity())
	{
		SetCapacity(m_Length);
	}
	Copy(start, m_Contents, m_Length); // handles terminator
}

template<typename CharType, typename Allocator>
template<typename OtherAllocator>
void String<CharType, Allocator>::Assign(const String<CharType, OtherAllocator> & string)
{
	Assign(string.Get(), string.GetEnd());
}

template<typename CharType, typename Allocator>
void String<CharType, Allocator>::Clear()
{
	if (m_Contents == s_EmptyStr)
	{
		return;
	}
	m_Contents[0] = (CharType)'\000';
	m_Length = 0;
}

template<typename CharType, typename Allocator>
void String<CharType, Allocator>::SetCapacity(size_t capacity)
{
	ASSERT(capacity >= 0);
	if (capacity == GetCapacity())
	{
		return;
	}
	else if (capacity == 0)
	{
		Deallocate();
		m_Contents = const_cast<CharType *>(s_EmptyCStr);
		m_MemShared = true;
	}
	else
	{
		if (m_MemShared)
		{
			Allocate(capacity + 1);
		}
		else
		{
			Reallocate(capacity + 1);
		}
		m_MemShared = false;
	}
	m_Capacity = capacity;
}

// operator += (CharType)
//------------------------------------------------------------------------------
template<typename CharType, typename Allocator>
String<CharType, Allocator> & String<CharType, Allocator>::operator += (CharType c)
{
	if (m_Length >= GetCapacity())
	{
		Grow(1);
	}
	m_Contents[m_Length++] = c;
	m_Contents[m_Length] = (CharType)'\000';

	return *this;
}

// operator += (const CharType *)
//-----------------------------------------------------------------------------
template<typename CharType, typename Allocator>
String<CharType, Allocator> & String<CharType, Allocator>::operator += (const CharType * string)
{
	size_t suffixLen = StrLen(string);
	size_t newLen = m_Length + suffixLen;
	if (newLen > GetCapacity())
	{
		Grow(suffixLen);
	}

	Copy(string, m_Contents + m_Length, suffixLen); // handles terminator
	m_Length += suffixLen;

	return *this;
}

template<typename CharType, typename Allocator>
template<typename OtherAllocator>
String<CharType, Allocator> & String<CharType, Allocator>::operator += (const String<CharType, OtherAllocator> & string)
{
	return ((*this) += string.Get());
}

template<typename CharType, typename Allocator>
void String<CharType, Allocator>::Append(const CharType * string, size_t len)
{
	uint32 newLen = m_Length + (uint32)len;
	if (newLen > GetCapacity())
	{
		Grow(len);
	}

	Copy(string, m_Contents + m_Length, len); // handles terminator
	m_Length = newLen;
}

template<typename CharType, typename Allocator>
void String<CharType, Allocator>::AppendFormat(const CharType * fmtString, ...)
{
	AStackString<1024> buffer;
	va_list args;
	va_start(args, fmtString);
	buffer.VFormat(fmtString, args);
	va_end(args);

	Append(buffer);
}

template<typename CharType, typename Allocator>
size_t String<CharType, Allocator>::Replace(CharType from, CharType to, size_t maxReplaces)
{
	size_t replaceCount = 0;
	CharType * pos = m_Contents;
	const CharType * end = m_Contents + m_Length;
	while (pos < end)
	{
		if (*pos == from)
		{
			*pos = to;
			replaceCount++;
			if (replaceCount == maxReplaces)
			{
				break;
			}
		}
		pos++;
	}
	return replaceCount;
}

template<typename CharType, typename Allocator>
void String<CharType, Allocator>::ToLower()
{
	CharType * pos = m_Contents;
	CharType * end = m_Contents + m_Length;
	while (pos < end)
	{
		CharType c = *pos;
		if ((c >= 'A') && (c <= 'Z'))
		{
			c = 'a' + (c - 'A');
			*pos = c;
		}
		pos++;
	}
}

template<typename CharType, typename Allocator>
void String<CharType, Allocator>::ToUpper()
{
	CharType * pos = m_Contents;
	CharType * end = m_Contents + m_Length;
	while (pos < end)
	{
		CharType c = *pos;
		if ((c >= 'a') && (c <= 'z'))
		{
			c = 'A' + (c - 'a');
			*pos = c;
		}
		pos++;
	}
}

template<typename CharType, typename Allocator>
size_t String<CharType, Allocator>::Replace(const CharType * from, const CharType * to, size_t maxReplaces)
{
	AStackString< 2 * KILOBYTE > temp;

	size_t replaceCount = 0;

	// Loop until the last possible position for a potential match
	const CharType * pos = m_Contents;
	const CharType * end = m_Contents + m_Length;
	const size_t fromLength = StrLen(from);
	while (pos <= (end - fromLength))
	{
		if (StrNCmp(pos, from, fromLength) == 0)
		{
			temp += to;
			pos += fromLength;
			maxReplaces--;
			replaceCount++;
			if (maxReplaces == 0)
			{
				break;
			}
		}
		else
		{
			temp += *pos;
			pos++;
		}
	}

	// Did we actually replace anything?
	if (replaceCount > 0)
	{
		// Copy remaining unmatchable CharTypeacters to the temp
		while (pos < end)
		{
			temp += *pos;
			pos++;
		}

		// Keep the result
		Assign(temp);
	}
	return replaceCount;
}

template<typename CharType, typename Allocator>
const CharType * String<CharType, Allocator>::Find(CharType c, const CharType * startPos) const
{
	// If startPos is provided, validate it
	// (deliberately allow startPos to point one past end of string)
	ASSERT((startPos == nullptr) || (startPos >= m_Contents));
	ASSERT((startPos == nullptr) || (startPos <= m_Contents + GetLength()));

	const CharType * pos = startPos ? startPos : m_Contents;
	const CharType * end = m_Contents + m_Length;
	while (pos < end)
	{
		if (*pos == c)
		{
			return pos;
		}
		pos++;
	}
	return nullptr;
}

template<typename CharType, typename Allocator>
const CharType * String<CharType, Allocator>::Find(const CharType * subString) const
{
	size_t subStrLen = StrLen(subString);

	const CharType * pos = m_Contents;
	const CharType * end = pos + m_Length - subStrLen;
	while (pos <= end)
	{
		if (StrNCmp(pos, subString, subStrLen) == 0)
		{
			return pos;
		}
		pos++;
	}
	return nullptr;
}

template<typename CharType, typename Allocator>
const CharType * String<CharType, Allocator>::FindI(const CharType * subString) const
{
	size_t subStrLen = StrLen(subString);

	const CharType * pos = m_Contents;
	const CharType * end = pos + m_Length - subStrLen;
	while (pos <= end)
	{
		if (StrNCmpI(pos, subString, subStrLen) == 0)
		{
			return pos;
		}
		pos++;
	}
	return nullptr;
}

template<typename CharType, typename Allocator>
const CharType * String<CharType, Allocator>::FindLast(CharType c) const
{
	const CharType * pos = m_Contents + m_Length - 1;
	while (pos >= m_Contents)
	{
		if (*pos == c)
		{
			return pos;
		}
		pos--;
	}
	return nullptr;
}

template<typename CharType, typename Allocator>
bool String<CharType, Allocator>::EndsWith(CharType c) const
{
	uint32 len = m_Length;
	if (len == 0)
	{
		return false;
	}
	return (m_Contents[len - 1] == c);
}

template<typename CharType, typename Allocator>
bool String<CharType, Allocator>::EndsWith(const CharType * string) const
{
	const size_t stringLen = StrLen(string);
	const CharType * possiblePos = m_Contents + m_Length - stringLen;
	if (possiblePos < m_Contents)
	{
		return false; // String to search is longer than this string
	}
	return (StrNCmp(possiblePos, string, stringLen) == 0);
}

template<typename CharType, typename Allocator>
template<typename OtherAllocator>
bool String<CharType, Allocator>::EndsWith(const String<CharType, OtherAllocator> & other) const
{
	const size_t otherLen = other.GetLength();
	if (otherLen > GetLength())
	{
		return false;
	}
	return (StrNCmp(GetEnd() - otherLen, other.Get(), otherLen) == 0);
}

template<typename CharType, typename Allocator>
bool String<CharType, Allocator>::EndsWithI(const CharType * other) const
{
	const size_t otherLen = StrLen(other);
	if (otherLen > GetLength())
	{
		return false;
	}
	return (StrNCmpI(GetEnd() - otherLen, other, otherLen) == 0);
}

template<typename CharType, typename Allocator>
template<typename OtherAllocator>
bool String<CharType, Allocator>::EndsWithI(const String<CharType, OtherAllocator> & other) const
{
	const size_t otherLen = other.GetLength();
	if (otherLen > GetLength())
	{
		return false;
	}
	return (StrNCmpI(GetEnd() - otherLen, other.Get(), otherLen) == 0);
}

template<typename CharType, typename Allocator>
bool String<CharType, Allocator>::BeginsWith(CharType c) const
{
	if (IsEmpty())
	{
		return false;
	}
	return (m_Contents[0] == c);
}

template<typename CharType, typename Allocator>
bool String<CharType, Allocator>::BeginsWith(const CharType * string) const
{
	size_t otherLen = StrLen(string);
	if (otherLen > GetLength())
	{
		return false;
	}
	return (StrNCmp(m_Contents, string, otherLen) == 0);
}

template<typename CharType, typename Allocator>
template<typename OtherAllocator>
bool String<CharType, Allocator>::BeginsWith(const String<CharType, OtherAllocator> & string) const
{
	uint32 otherLen = string.GetLength();
	if (otherLen > GetLength())
	{
		return false;
	}
	return (StrNCmp(m_Contents, string.Get(), otherLen) == 0);
}

template<typename CharType, typename Allocator>
bool String<CharType, Allocator>::BeginsWithI(const CharType * string) const
{
	size_t otherLen = StrLen(string);
	if (otherLen > GetLength())
	{
		return false;
	}
	return (StrNCmpI(m_Contents, string, otherLen) == 0);
}

template<typename CharType, typename Allocator>
template<typename OtherAllocator>
bool String<CharType, Allocator>::BeginsWithI(const String<CharType, OtherAllocator> & string) const
{
	uint32 otherLen = string.GetLength();
	if (otherLen > GetLength())
	{
		return false;
	}
	return (StrNCmpI(m_Contents, string.Get(), otherLen) == 0);
}

// Match
// NOTE: This code is based on that in the C/C++ Users Journal (Mike Cornelison)
//------------------------------------------------------------------------------
template<typename CharType, typename Allocator>
/*static*/ bool String<CharType, Allocator>::Match(const CharType * pat, const CharType * str)
{
	bool star;

new_segment:
	star = false;
	if (*pat == '*')
	{
		star = true;
		do { pat++; } while (*pat == '*');
	}

test_match:
	int i;
	for (i = 0; pat[i] && (pat[i] != '*'); i++)
	{
		CharType a = str[i];
		CharType b = pat[i];
		if (a != b)
		{
			if (!str[i]) return false;
			if ((pat[i] == '?') && (str[i] != '.')) continue;
			if (!star) return false;
			str++;
			goto test_match;
		}
	}
	if (pat[i] == '*')
	{
		str += i;
		pat += i;
		goto new_segment;
	}
	if (!str[i]) return true;
	if (i && pat[i - 1] == '*') return true;
	if (!star) return false;
	str++;
	goto test_match;
}

// MatchI
// NOTE: This code is based on that in the C/C++ Users Journal (Mike Cornelison)
//------------------------------------------------------------------------------
template<typename CharType, typename Allocator>
/*static*/ bool String<CharType, Allocator>::MatchI(const CharType * pat, const CharType * str)
{
	bool star;

new_segment:
	star = false;
	if (*pat == '*')
	{
		star = true;
		do { pat++; } while (*pat == '*');
	}

test_match:
	int i;
	for (i = 0; pat[i] && (pat[i] != '*'); i++)
	{
		CharType a = str[i]; a = ((a >= 'A') && (a <= 'Z')) ? 'a' + (a - 'A') : a;
		CharType b = pat[i]; b = ((b >= 'A') && (b <= 'Z')) ? 'a' + (b - 'A') : b;
		if (a != b)
		{
			if (!str[i]) return false;
			if ((pat[i] == '?') && (str[i] != '.')) continue;
			if (!star) return false;
			str++;
			goto test_match;
		}
	}
	if (pat[i] == '*')
	{
		str += i;
		pat += i;
		goto new_segment;
	}
	if (!str[i]) return true;
	if (i && pat[i - 1] == '*') return true;
	if (!star) return false;
	str++;
	goto test_match;
}

template<typename CharType, typename Allocator>
/*static*/ void String<CharType, Allocator>::Copy(const CharType * src, CharType * dst, size_t len)
{
	if (len == 0)
	{
		return;
	}
	const CharType * end = src + len;
	while (src < end)
	{
		*dst++ = *src++;
	}
	*dst = (CharType)'\000';
}

template<typename CharType, typename Allocator>
/*static*/ size_t String<CharType, Allocator>::StrLen(const CharType * string)
{
	const CharType * pos = string;
	while (*pos != (CharType)'\000')
	{
		pos++;
	}
	return (pos - string);
}

template<typename CharType, typename Allocator>
/*static*/ uint32 String<CharType, Allocator>::StrNCmp(const CharType * a, const CharType * b, size_t num)
{
	while (num > 0)
	{
		if (*a == *b)
		{
			if (*a == (CharType)'\000')
			{
				return 0; // Both strings ended and equal
			}

			// Keep going
			a++;
			b++;
			num--;
			continue;
		}

		// Different, so return relationship
		return (*a > *b) ? 1 : -1;
	};
	return 0; // Strings identical upto 'num' CharTypes
}

template<typename CharType, typename Allocator>
/*static*/ uint32 String<CharType, Allocator>::StrNCmpI(const CharType * a, const CharType * b, size_t num)
{
	while (num > 0)
	{
		CharType a1 = *a;
		if ((a1 >= 'A') && (a1 <= 'Z'))
		{
			a1 = 'a' + (a1 - 'A');
		}
		CharType b1 = *b;
		if ((b1 >= 'A') && (b1 <= 'Z'))
		{
			b1 = 'a' + (b1 - 'A');
		}
		if (a1 == b1)
		{
			if (a1 == (CharType)'\000')
			{
				return 0; // Both strings ended and equal
			}

			// Keep going
			a++;
			b++;
			num--;
			continue;
		}

		// Different, so return relationship
		return (a1 - b1);
	};
	return 0; // Strings identical upto 'num' CharTypes
}

template<typename CharType, typename Allocator>
template<typename OtherAllocator>
void String<CharType, Allocator>::Tokenize(Array<String<CharType, OtherAllocator>> & tokens, CharType splitChar) const
{
	Array<const CharType *> tokenStarts;
	Array<const CharType *> tokenEnds;

	const CharType * pos = Get();
	const CharType * end = GetEnd();
	bool lookingForStart = true;
	CharType quoteChar = 0;
	while (pos < end)
	{
		if (lookingForStart)
		{
			if (*pos == splitChar)
			{
				++pos;
				continue;
			}

			// found the start of a new token
			tokenStarts.Append(pos);
			lookingForStart = false;
		}

		// hit a quote?
		CharType c = *pos;
		if ((c == '"') || (c == '\''))
		{
			if (quoteChar == 0)
			{
				// opening quote
				quoteChar = c;
			}
			else if (quoteChar == c)
			{
				// closing quote
				quoteChar = 0;
			}
			else
			{
				// quote of the 'other' type - consider as part of token
			}
		}
		else if (c == splitChar)
		{
			if (quoteChar == 0)
			{
				tokenEnds.Append(pos);
				lookingForStart = true;
			}
			else
			{
				// space inside quoted token - consider as part of token
			}
		}
		else
		{
			// normal CharTypeacter part of token
		}
		++pos;
	}
	ASSERT((tokenStarts.GetSize() == tokenEnds.GetSize()) ||
		(tokenStarts.GetSize() == (tokenEnds.GetSize() + 1)));
	if (tokenStarts.GetSize() > tokenEnds.GetSize())
	{
		tokenEnds.Append(pos);
	}
	ASSERT(tokenStarts.GetSize() == tokenEnds.GetSize());

	// pre-size output to avoid reallocations
	tokens.Clear();
	const size_t numTokens(tokenStarts.GetSize());
	if (tokens.GetCapacity() < numTokens)
	{
		tokens.SetCapacity(numTokens);
	}
	tokens.SetSize(numTokens);

	// copy tokens
	for (size_t i = 0; i < numTokens; ++i)
	{
		tokens[i].Assign(tokenStarts[i], tokenEnds[i]);
	}
}

template<typename CharType, typename Allocator>
void String<CharType, Allocator>::Grow(size_t addedSize = 0)
{
	// Grow by 1.5 times
	size_t newCapacity = Math::Max(m_Capacity + addedSize, m_Capacity + (m_Capacity >> 1) + 1);
	if (m_MemShared)
	{
		Allocate(newCapacity);
	}
	else
	{
		Reallocate(newCapacity);
	}
	m_MemShared = false;
}

template<typename CharType, typename Allocator>
void String<CharType, Allocator>::Allocate(size_t size)
{
	ASSERT(m_Contents == s_EmptyCStr);
	m_Contents = (CharType *)m_AllocatorInst.Allocate(size * sizeof(CharType));
}

template<typename CharType, typename Allocator>
void String<CharType, Allocator>::Reallocate(size_t size)
{
	ASSERT(size > 0);
	m_Contents = (CharType *)m_AllocatorInst.Reallocate(m_Contents, size * sizeof(CharType));
}

template<typename CharType, typename Allocator>
void String<CharType, Allocator>::Deallocate()
{
	ASSERT(m_Contents);
	if (!m_MemShared)
	{
		m_AllocatorInst.Free(m_Contents);
	}
}

//------------------------------------------------------------------------------