// BitArray.inl
//------------------------------------------------------------------------------

// BitArrayElement
//------------------------------------------------------------------------------
template<bool Const>
BitArrayElement<Const>::BitArrayElement(InnerType& element, InnerType mask)
	: m_Element(element)
	, m_Mask(mask)
{}

template<bool Const>
BitArrayElement<Const>::operator bool() const
{
	return (m_Element & m_Mask) != 0;
}

template<bool Const>
bool BitArrayElement<Const>::operator==(bool bValue) const
{
	return bool(*this) == bValue;
}

template<bool Const>
bool BitArrayElement<Const>::operator!=(bool bValue) const
{
	return bool(*this) != bValue;
}

template<bool Const>
template<bool rConst>
bool BitArrayElement<Const>::operator==(const BitArrayElement<rConst>& rOther) const
{
	return bool(*this) == bool(rOther);
}

template<bool Const>
template<bool rConst>
bool BitArrayElement<Const>::operator!=(const BitArrayElement<rConst>& rOther) const
{
	return bool(*this) != bool(rOther);
}


// BitArrayIterator
//------------------------------------------------------------------------------
template<bool Const>
BitArrayIterator<Const>::BitArrayIterator(InnerType* element, InnerType mask)
	: m_Element(element)
	, m_Mask(mask)
{
}

template<bool Const>
BitArrayIterator<Const>::BitArrayIterator(const BitArrayIterator<!Const>& rOther)
{
	m_Element = rOther.m_Element;
	m_Mask = rOther.m_Mask;
}

template<bool Const>
typename BitArrayIterator<Const>::ConstElement BitArrayIterator<Const>::operator*() const
{
	return ConstElement(*m_Element, m_Mask);
}

template<bool Const>
template<class>
typename BitArrayIterator<Const>::Element BitArrayIterator<Const>::operator*()
{
	return Element(*m_Element, m_Mask);
}

template<bool Const>
BitArrayIterator<Const>& BitArrayIterator<Const>::operator++()
{
	if (!(m_Mask <<= 1))
	{
		++m_Element;
		m_Mask = 1;
	}
	return *this;
}

template<bool Const>
BitArrayIterator<Const> BitArrayIterator<Const>::operator++(int)
{
	BitArrayIterator<Const> result = *this;
	++(*this);
	return result;
}

template<bool Const>
BitArrayIterator<Const>& BitArrayIterator<Const>::operator--()
{
	if (!(m_Mask >>= 1))
	{
		--m_Element;
		m_Mask = 1 << (EBITSIZE - 1);
	}
	return *this;
}

template<bool Const>
BitArrayIterator<Const> BitArrayIterator<Const>::operator--(int)
{
	BitArrayIterator<Const> result = *this;
	--(*this);
	return result;
}

template<bool Const>
bool BitArrayIterator<Const>::operator==(const BitArrayIterator<Const>& rOther) const
{
	return m_Element == rOther.m_Element && m_Mask == rOther.m_Mask;
}

template<bool Const>
bool BitArrayIterator<Const>::operator!=(const BitArrayIterator<Const>& rOther) const
{
	return m_Element != rOther.m_Element || m_Mask != rOther.m_Mask;
}

template<bool Const>
bool BitArrayIterator<Const>::operator<(const BitArrayIterator<Const>& rOther) const
{
	return m_Element < rOther.m_Element || (m_Element == rOther.m_Element && m_Mask < rOther.m_Mask);
}

template<bool Const>
bool BitArrayIterator<Const>::operator>(const BitArrayIterator<Const>& rOther) const
{
	return m_Element > rOther.m_Element || (m_Element == rOther.m_Element && m_Mask > rOther.m_Mask);
}

template<bool Const>
bool BitArrayIterator<Const>::operator<=(const BitArrayIterator<Const>& rOther) const
{
	return m_Element < rOther.m_Element || (m_Element == rOther.m_Element && m_Mask <= rOther.m_Mask);
}

template<bool Const>
bool BitArrayIterator<Const>::operator>=(const BitArrayIterator<Const>& rOther) const
{
	return m_Element > rOther.m_Element || (m_Element == rOther.m_Element && m_Mask >= rOther.m_Mask);
}


// BitArray
//------------------------------------------------------------------------------
template<class Allocator>
BitArray<Allocator>::BitArray()
	: m_Size(0)
{}

template<class Allocator>
BitArray<Allocator>::BitArray(const Allocator& allocator)
	: m_Size(0)
	, m_Content(allocator)
{
}

template<class Allocator>
BitArray<Allocator>::BitArray(SIZET initSize)
	: m_Size(initSize)
	, m_Content(GetInnerIndex(initSize - 1) + 1)
{
}

template<class Allocator>
template<class OtherAllocator>
BitArray<Allocator>::BitArray(const BitArray<OtherAllocator>& other)
{
	m_Size = other.m_Size;
	m_Content = other.m_Content;
}

// operators
template<class Allocator>
template<class OtherAllocator>
BitArray<Allocator>& BitArray<Allocator>::operator=(const BitArray<OtherAllocator>& other)
{
	m_Size = other.m_Size;
	m_Content = other.m_Content;
}

// iterator
template<class Allocator>
typename BitArray<Allocator>::Iter BitArray<Allocator>::Begin()
{
	return Iter(&m_Content[GetInnerIndex(0)], 1 << GetInnerOffset(0));
}

template<class Allocator>
typename BitArray<Allocator>::Iter BitArray<Allocator>::End()
{
	return Iter(&m_Content[GetInnerIndex(m_Size)], 1 << GetInnerOffset(m_Size));
}

template<class Allocator>
typename BitArray<Allocator>::ConstIter BitArray<Allocator>::Begin() const
{
	return ConstIter(&m_Content[GetInnerIndex(0)], 1 << GetInnerOffset(0));
}

template<class Allocator>
typename BitArray<Allocator>::ConstIter BitArray<Allocator>::End() const
{
	return ConstIter(&m_Content[GetInnerIndex(m_Size)], 1 << GetInnerOffset(m_Size));
}


// access
template<class Allocator>
typename BitArray<Allocator>::Element BitArray<Allocator>::operator[](SIZET index)
{
	return Element(m_Content[GetInnerIndex(index)], 1 << GetInnerOffset(index));
}

template<class Allocator>
typename BitArray<Allocator>::ConstElement BitArray<Allocator>::operator[](SIZET index) const
{
	return ConstElement(m_Content[GetInnerIndex(index)], 1 << GetInnerOffset(index));
}

template<class Allocator>
void BitArray<Allocator>::Set(SIZET index, bool value)
{
	ASSERT(index < m_Size);
	ASSERT(index < m_Content.GetSize() * 8);

	SIZET innerIdx = GetInnerIndex(index);
	SIZET innerOffset = GetInnerOffset(index);

	if (value)
	{
		m_Content[innerIdx] |= (1 << innerOffset);
	}
	else
	{
		m_Content[innerIdx] &= ~(1 << innerOffset);
	}
}

template<class Allocator>
void BitArray<Allocator>::Toggle(SIZET index)
{
	ASSERT(index < m_Size);
	ASSERT(index < m_Content.GetSize() * EBITSIZE);

	SIZET innerIdx = GetInnerIndex(index);
	SIZET innerOffset = GetInnerOffset(index);
	m_Content[innerIdx] ^= (1 << innerOffset);
}

template<class Allocator>
void BitArray<Allocator>::SetAll(bool value)
{
	ASSERT(GetInnerIndex(m_Size - 1) + 1 == m_Content.GetSize());
	InnerType * inner = m_Content.GetPointer();
	MemSet(inner, value ? 0xFF : 0x00, (GetInnerIndex(m_Size - 1) + 1) * sizeof(InnerType));
}

template<class Allocator>
void BitArray<Allocator>::ToggleAll()
{
	ASSERT(GetInnerIndex(m_Size - 1) + 1 == m_Content.GetSize());
	for (InnerArray::Iter iter = m_Content.Begin(); iter != m_Content.End(); ++iter)
	{
		(*iter) = ~(*iter);
	}
}

template<class Allocator>
void BitArray<Allocator>::SetRange(SIZET begin, SIZET end, bool value)
{
	ASSERT(begin >= 0 && begin < m_Size);
	ASSERT(end > 0 && end <= m_Size);

	SIZET leftIdx = GetInnerIndex(begin);
	SIZET leftOffset = GetInnerOffset(begin);
	SIZET rightIdx = GetInnerIndex(end - 1);
	SIZET rightOffset = GetInnerOffset(end - 1);

	InnerType * inner = m_Content.GetPointer();	
	if (rightIdx > leftIdx)
	{
		SetInnerRange(leftIdx, leftOffset, EBITSIZE, value);
		if (rightIdx - leftIdx > 1)
		{
			MemSet(inner + leftIdx + 1, value ? 0xFF : 0x00, (rightIdx - leftIdx) * sizeof(InnerType));
		}
		SetInnerRange(rightIdx, 0, rightOffset + 1, value);
	}
	else
	{
		SetInnerRange(leftIdx, leftOffset, rightOffset + 1, value);
	}
}

template<class Allocator>
void BitArray<Allocator>::ToggleRange(SIZET beign, SIZET end)
{
	ASSERT(begin >= 0 && begin < m_Size);
	ASSERT(end > 0 && end <= m_Size);

	SIZET leftIdx = GetInnerIndex(begin);
	SIZET leftOffset = GetInnerOffset(begin);
	SIZET rightIdx = GetInnerIndex(end - 1);
	SIZET rightOffset = GetInnerOffset(end - 1);

	InnerType * inner = m_Content.GetPointer();
	if (rightIdx > leftIdx)
	{
		ToggleInnerRange(leftIdx, leftOffset, EBITSIZE);
		for (SIZET idx = 1; idx < rightIdx - leftIdx; ++idx)
		{
			m_Content[idx] ~= m_Content[idx];
		}
		ToggleInnerRange(rightIdx, 0, rightOffset + 1);
	}
	else
	{
		ToggleInnerRange(leftIdx, leftOffset, rightOffset + 1);
	}
}

template<class Allocator>
void BitArray<Allocator>::SetInnerRange(SIZET index, SIZET begin, SIZET end, bool value)
{
	// bitset: begin -> end - 1
	ASSERT(begin >= 0 && begin < EBITSIZE);
	ASSERT(end > 0 && end <= EBITSIZE);
	if (value)
	{
		m_Content[index] |= ((0xFFFFF << begin) - (0xFFFF << end));
	}
	else
	{
		m_Content[index] &= ~((0xFFFFF << begin) - (0xFFFF << end));
	}
}

template<class Allocator>
void BitArray<Allocator>::ToggleInnerRange(SIZET index, SIZET begin, SIZET end)
{
	// bitset: begin -> end - 1
	ASSERT(begin >= 0 && begin < EBITSIZE);
	ASSERT(end > 0 && end <= EBITSIZE);
	if (value)
	{
		m_Content[index] ^= ~((0xFFFFF << begin) - (0xFFFF << end));
	}
	else
	{
		m_Content[index] ^= ((0xFFFFF << begin) - (0xFFFF << end));
	}
}

template<class Allocator>
void BitArray<Allocator>::Append(bool value, SIZET count = 1)
{
	ASSERT(GetInnerIndex(m_Size - 1) + 1 == m_Content.GetSize());

	SIZET newSize = m_Size + count;
	SIZET newInnerSize = GetInnerIndex(newSize - 1) + 1;
	m_Content.SetSize(newInnerSize);
	m_Size = newSize;
	SetRange(m_Size - count, m_Size, value);
}

template<class Allocator>
void BitArray<Allocator>::SetSize(SIZET size)
{
	m_Size = size;
	m_Content.SetSize(GetInnerIndex(size - 1) + 1);
}