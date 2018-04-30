// String.inl
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include <utility>


// Constructor
//------------------------------------------------------------------------------
template<typename CharType, typename Allocator>
String<CharType, Allocator>::String()
	: m_Contents(const_cast<CharType *>(GetEmptyCStr<CharType>()))
	, m_Length(0)
	, m_Capacity(0)
{
}

template<typename CharType, typename Allocator>
String<CharType, Allocator>::String(const CharType * string)
	: m_Contents(const_cast<CharType *>(GetEmptyCStr<CharType>()))
	, m_Capacity(0)
{
	ASSERT(string);
	m_Length = StrLen(string);
	SetCapacity(m_Length, false);
	Copy(string, m_Contents, m_Length); // copy handles terminator
}

template<typename CharType, typename Allocator>
String<CharType, Allocator>::String(const CharType * start, const CharType * end)
	: m_Contents(const_cast<CharType *>(GetEmptyCStr<CharType>()))
	, m_Capacity(0)
{
	ASSERT(start && end);
	m_Length = (SIZET)(end - start);
	SetCapacity(m_Length, false);
	Copy(start, m_Contents, m_Length); // copy handles terminator
}

// Copy Constructor
//------------------------------------------------------------------------------
template<typename CharType, typename Allocator>
String<CharType, Allocator>::String(const String & string)
	: m_Contents(const_cast<CharType *>(GetEmptyCStr<CharType>()))
	, m_Capacity(0)
{
	m_Length = string.GetLength();
	SetCapacity(m_Length, false);
	Copy(string.Get(), m_Contents, m_Length); // copy handles terminator
}

template<typename CharType, typename Allocator>
template<class>
String<CharType, Allocator>::String(String && rOther)
	: Allocator(std::move(rOther))
	, m_Contents(rOther.m_Contents)
	, m_Length(rOther.m_Length)
	, m_Capacity(rOther.m_Capacity)
{
	rOther.m_Length = 0;
	rOther.m_Capacity = 0;
	rOther.m_Contents = const_cast<CharType*>(GetEmptyCStr<CharType>());
}

template<typename CharType, typename Allocator>
template<typename OtherAllocator>
String<CharType, Allocator>::String(const String<CharType, OtherAllocator> & string)
	: m_Contents(const_cast<CharType *>(GetEmptyCStr<CharType>()))
	, m_Capacity(0)
{
	m_Length = string.GetLength();
	SetCapacity(m_Length, false);
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
template<class>
String<CharType, Allocator> & String<CharType, Allocator>::operator = (String && rOther)
{
	Allocator::operator=(std::move(rOther));
	Swap(rOther);
	return *this;
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
	m_Length = (SIZET)(end - start);
	if (m_Length > GetCapacity())
	{
		SetCapacity(m_Length, false);
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
	if (m_Contents == s_EmptyCStr)
	{
		return;
	}
	m_Contents[0] = (CharType)'\000';
	m_Length = 0;
}

template<typename CharType, typename Allocator>
void String<CharType, Allocator>::SetCapacity(SIZET capacity)
{
	SetCapacity(capacity, true);
}

template<typename CharType, typename Allocator>
void String<CharType, Allocator>::SetLength(SIZET length)
{
	SetCapacity(length, true);
	m_Length = length;
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
	SIZET suffixLen = StrLen(string);
	SIZET newLen = m_Length + suffixLen;
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
void String<CharType, Allocator>::Append(const CharType * string, SIZET len)
{
	SIZET newLen = m_Length + len;
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
void String<CharType, Allocator>::AppendMultiply(const CharType * string, SIZET count)
{
	SIZET newSize = m_Length + StrLen(string) * count;
	if (m_Capacity < newSize)
	{
		SetCapacity(newSize);
	}
	
	for (SIZET idx = 0; idx < count; ++idx)
	{
		Append(string, StrLen(string));
	}
}

template<typename CharType, typename Allocator>
SIZET String<CharType, Allocator>::Replace(CharType from, CharType to, SIZET maxReplaces)
{
	SIZET replaceCount = 0;
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
SIZET String<CharType, Allocator>::Replace(const CharType * from, const CharType * to, SIZET maxReplaces)
{
	AStackString< 2 * KILOBYTE > temp;

	SIZET replaceCount = 0;

	// Loop until the last possible position for a potential match
	const CharType * pos = m_Contents;
	const CharType * end = m_Contents + m_Length;
	const SIZET fromLength = StrLen(from);
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
void String<CharType, Allocator>::Swap(String& other)
{
	std::swap(m_Contents, other.m_Contents);
	std::swap(m_Length, other.m_Length);
	std::swap(m_Capacity, other.m_Capacity);
}

template<typename CharType, typename Allocator>
void String<CharType, Allocator>::Grow(SIZET addedSize = 0)
{
	// Grow by 1.5 times
	SIZET newCapacity = Math::Max(m_Capacity + addedSize, m_Capacity + (m_Capacity >> 1));
	SetCapacity(newCapacity, true);
}

template<typename CharType, typename Allocator>
void String<CharType, Allocator>::SetCapacity(SIZET capacity, bool needCopy)
{
	if (m_Capacity != capacity)
	{
		SIZET newLength = Math::Min(m_Length, capacity);
		if (capacity == 0)
		{
			Deallocate();
			m_Contents = const_cast<CharType *>(s_EmptyCStr);
		}
		else
		{
			CharType * newMem = Allocate(capacity + 1);
			if (needCopy)
			{
				Array<CharType>::UninitializedCopy(newMem, m_Contents, newLength);
				Array<CharType>::InPlaceDestruct(m_Contents, m_Length);
			}
			Deallocate();
			m_Contents = newMem;
			m_Contents[capacity] = '\000';
		}
		m_Length = newLength;
		m_Capacity = capacity;
	}
}

template<typename CharType, typename Allocator>
CharType * String<CharType, Allocator>::Allocate(SIZET size)
{
	ASSERT(size > 0);
	return AllocateT<CharType>(size);
}

template<typename CharType, typename Allocator>
CharType * String<CharType, Allocator>::Reallocate(SIZET size)
{
	ASSERT(size > 0);
	return ReallocateT<CharType>(m_Contents, size);
}

template<typename CharType, typename Allocator>
void String<CharType, Allocator>::Deallocate()
{
	if (m_Contents && m_Capacity > 0)
	{
		FreeT(m_Contents);
	}
}

//------------------------------------------------------------------------------