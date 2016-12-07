// Array functions
//------------------------------------------------------------------------------

// Constructors
//------------------------------------------------------------------------------
template<class T, class Allocator>
Array<T, Allocator>::Array()
	: m_Begin(nullptr)
	, m_End(nullptr)
	, m_MaxEnd(nullptr)
{
}

template<class T, class Allocator>
Array<T, Allocator>::Array(size_t initSize)
	: m_Begin(nullptr)
	, m_End(nullptr)
	, m_MaxEnd(nullptr)
{
	SetSize(initSize);
}

template<class T, class Allocator>
Array<T, Allocator>::Array(const T * begin, const T * end)
	: m_Begin(nullptr)
	, m_End(nullptr)
	, m_MaxEnd(nullptr)
{
	const size_t size = (end - begin);
	SetCapacity(size);
	UninitializedCopy(m_Begin, begin, size);
	m_End = m_Begin + size;
}

// Copy Constructors
//------------------------------------------------------------------------------
template<class T, class Allocator>
Array<T, Allocator>::Array(const Array<T, Allocator> & other)
	: m_Begin(nullptr)
	, m_End(nullptr)
	, m_MaxEnd(nullptr)
{
	const size_t size = other.GetSize();
	SetCapacity(other.GetCapacity());
	UninitializedCopy(m_Begin, other.m_Begin, size);
	m_End = m_Begin + size;
}

template<class T, class Allocator>
template<class OtherAllocator>
Array<T, Allocator>::Array(const Array<T, OtherAllocator> & other)
	: m_Begin(nullptr)
	, m_End(nullptr)
	, m_MaxEnd(nullptr)
{
	const size_t size = other.GetSize();
	SetCapacity(other.GetCapacity());
	UninitializedCopy(m_Begin, other.m_Begin, size);
	m_End = m_Begin + size;
}

// Destructor
//-----------------------------------------------------------------------------
template<class T, class Allocator>
Array<T, Allocator>::~Array()
{
	InPlaceDestruct(m_Begin, m_End - m_Begin);
	Deallocate();
}

template<class T, class Allocator>
void Array<T, Allocator>::SetCapacity(size_t capacity)
{
	Resize(GetSize(), capacity);
}

template<class T, class Allocator>
void Array<T, Allocator>::SetSize(size_t size)
{
	Resize(size, Math::Max(size, GetCapacity()));
}

template<class T, class Allocator>
void Array<T, Allocator>::Clear()
{
	InPlaceDestruct(m_Begin, m_End - m_Begin);
	m_End = m_Begin;
}

template<class T, class Allocator>
void Array<T, Allocator>::Swap(Array<T> & other)
{
	T * tmpBegin = m_Begin;
	T * tmpEnd = m_End;
	T * tmpMaxEnd = m_MaxEnd;
	m_Begin = other.m_Begin;
	m_End = other.m_End;
	m_MaxEnd = other.m_MaxEnd;
	other.m_Begin = tmpBegin;
	other.m_End = tmpEnd;
	other.m_MaxEnd = tmpMaxEnd;
}

template<class T, class Allocator>
void Array<T, Allocator>::SwapItem(size_t index1, size_t index2)
{
	ASSERT(index1 >= 0 && index1 < GetSize());
	ASSERT(index2 >= 0 && index2 < GetSize());

	T tmp = *(m_Begin[index1]);
	m_Begin[index1] = m_Begin[index2];
	m_Begin[index2] = tmp;
}

template<class T, class Allocator>
void Array<T, Allocator>::SwapItem(Iter& iter1, Iter& iter2)
{
	ASSERT(iter1 != End());
	ASSERT(iter2 != End());

	T tmp = *(iter1);
	(*iter1) = (*iter2);
	(*iter2) = tmp;
}

template<class T, class Allocator>
template<class U>
T * Array<T, Allocator>::Find(const U & obj) const
{
	T * pos = m_Begin;
	T * end = m_End;
	while (pos < end)
	{
		if (*pos == obj)
		{
			return pos;
		}
		pos++;
	}
	return nullptr;
}

template<class T, class Allocator>
template<class U>
T * Array<T, Allocator>::Find(const U & obj, bool(*equal)(const T &, const U &)) const
{
	T * pos = m_Begin;
	T * end = m_End;
	while (pos < end)
	{
		if (equal(*pos, obj))
		{
			return pos;
		}
		pos++;
	}
	return nullptr;
}

template<class T, class Allocator>
template<class U>
size_t Array<T, Allocator>::FindPos(const U & obj) const
{
	T * pos = m_Begin;
	T * end = m_End;
	while (pos < end)
	{
		if (*pos == obj)
		{
			return pos - m_Begin;
		}
		pos++;
	}
	return m_End - m_Begin;
}

template<class T, class Allocator>
template<class U>
T * Array<T, Allocator>::FindDeref(const U & obj) const
{
	T * pos = m_Begin;
	T * end = m_End;
	while (pos < end)
	{
		if (*(*pos) == obj)
		{
			return pos;
		}
		pos++;
	}
	return nullptr;
}

// Append
//------------------------------------------------------------------------------
template<class T, class Allocator>
void Array<T, Allocator>::Append(const T & item, size_t count)
{
	const size_t newSize = GetSize() + count;
	Grow(newSize);
	UninitializedFill(m_End, item, count);
	m_End += count;
}

// Append
//------------------------------------------------------------------------------
template<class T, class Allocator>
template<class U>
void Array<T, Allocator>::Append(const Array<U> & other)
{
	const size_t newSize = GetSize() + other.GetSize();
	Grow(newSize);
	UninitializedCopy(m_End, other.m_Begin, other.GetSize());
	m_End += other.GetSize();
}

// Append
//------------------------------------------------------------------------------
template<class T, class Allocator>
template<class U>
void Array<T, Allocator>::Append(const U * begin, const U * end)
{
	const size_t newSize = GetSize() + (end - begin);
	Grow(newSize);
	UninitializedCopy(m_End, begin, end - begin);
	m_End += (end - begin);
}

// FindOrAppend
//------------------------------------------------------------------------------
template<class T, class Allocator>
size_t Array<T, Allocator>::FindOrAppend(const T & item)
{
	const T* iter = Find(item);
	if (!iter)
	{
		Append(item);
		return m_End - m_Begin - 1;
	}
	return iter - m_Begin;
}

// Insert
//------------------------------------------------------------------------------
template<class T, class Allocator>
void Array<T, Allocator>::Insert(size_t index, const T & item, size_t count)
{
	ASSERT(index >= 0 && index <= GetSize());

	const size_t oldSize = GetSize();
	const size_t newSize = oldSize + count;

	if (newSize > GetCapacity())
	{
		size_t newCapacity = GetGrowCapacity(newSize);
		T * newMem = Reallocate(newCapacity);

		UninitializedCopy(newMem, m_Begin, index);
		UninitializedFill(newMem + index, item, count);
		UninitializedCopy(newMem + index + count, m_Begin + index, oldSize - index);

		Deallocate();

		m_Begin = newMem;
		m_End = m_Begin + oldSize + count;
		m_MaxEnd = m_Begin + newCapacity;
	}
	else
	{
		T * posInsert = m_Begin + index;
		const size_t shiftCount = m_End - posInsert;
		
		if (shiftCount < count)
		{
			UninitializedCopy(posInsert + count, posInsert, shiftCount);
			UninitializedFill(m_End, item, count - shiftCount);
			Set(posInsert, item, shiftCount);
		}
		else
		{
			UninitializedCopy(m_End, m_End - count, count);
			Move(posInsert + count, posInsert + index, shiftCount - count);
			Set(posInsert, item, count);
		}

		m_End = m_Begin + oldSize + count;
	}
}

// Insert
//------------------------------------------------------------------------------
template<class T, class Allocator>
void Array<T, Allocator>::Insert(Iter iter, const T & item, size_t count)
{
	Insert(iter - m_Begin, item, count);
}

// Pop
//------------------------------------------------------------------------------
template<class T, class Allocator>
void Array<T, Allocator>::Pop(size_t count)
{
	Erase(m_End - count, count);
}

// PopFront
//------------------------------------------------------------------------------
template<class T, class Allocator>
void Array<T, Allocator>::PopFront(size_t count)
{
	Erase(m_Begin, count);
}

// Erase (iter)
//------------------------------------------------------------------------------
template<class T, class Allocator>
void Array<T, Allocator>::Erase(T * const iter, size_t count)
{
	ASSERT(iter >= m_Begin && iter < m_End);
	ASSERT(iter + count <= m_End);

	Move(iter, iter + count, m_End - iter - 1);
	InPlaceDestruct(m_End - count, count);
	m_End -= count;
}

// Operator =
//------------------------------------------------------------------------------
template<class T, class Allocator>
Array<T, Allocator> & Array<T, Allocator>::operator = (const Array<T> & other)
{
	if (this != &other)
	{
		InPlaceDestruct(m_Begin, m_End - m_Begin);

		// don't shrink capacity to other capacity
		const size_t newSize = other.GetSize();
		if (newSize > GetCapacity())
		{
			T * newMem = Reallocate(newSize);
			Deallocate();
			m_Begin = newMem;
			m_MaxEnd = m_Begin + newSize;
		}

		UninitializedCopy(m_Begin, other.m_Begin, newSize);
		m_End = m_Begin + newSize;
	}
	return *this;
}

template<class T, class Allocator>
template<class OtherAllocator>
Array<T, Allocator> & Array<T, Allocator>::operator = (const Array<T, OtherAllocator> & other)
{
	if (this != &other)
	{
		InPlaceDestruct(m_Begin, m_End - m_Begin);

		// don't shrink capacity to other capacity
		const size_t newSize = other.GetSize();
		if (newSize > GetCapacity())
		{
			T * newMem = Reallocate(newSize);
			Deallocate();
			m_Begin = newMem;
			m_MaxEnd = m_Begin + newSize;
		}

		UninitializedCopy(m_Begin, other.m_Begin, newSize);
		m_End = m_Begin + newSize;
	}
	return *this;
}

// EraseSwap
//-----------------------------------------------------------------------------
template<class T, class Allocator>
void Array<T, Allocator>::EraseSwapIndex(size_t index, size_t count)
{
	ASSERT(index <= GetSize());
	ASSERT(index + count <= GetSize());

	size_t shiftStartIndex = index + count;
	size_t trailingCount = GetSize() - shiftStartIndex;
	m_End -= count;
	
	if (trailingCount <= count)
	{
		// We're removing more elements from the array than exist past the end of the range being removed, so
		// perform a normal shift and destroy.
		Move(m_Begin + index, m_Begin + shiftStartIndex, trailingCount);
	}
	else
	{
		// Swap elements from the end of the array into the empty space.
		Copy(m_Begin + index, m_End, count);
	}

	InPlaceDestruct(m_End, count);
}

// Shrink
//------------------------------------------------------------------------------
template<class T, class Allocator>
void Array<T, Allocator>::Shrink()
{
	SetCapacity(GetSize());
}

// GetGrowCapacity
//------------------------------------------------------------------------------
template<class T, class Allocator>
size_t Array<T, Allocator>::GetGrowCapacity(size_t capacity)
{
	size_t currentCapacity = m_MaxEnd - m_Begin;
	return Math::Max(capacity, currentCapacity + (currentCapacity >> 1) + 1);
}

// Grow
//------------------------------------------------------------------------------
template<class T, class Allocator>
void Array<T, Allocator>::Grow(size_t capacity)
{
	if (capacity > GetCapacity())
	{
		Resize(GetSize(), GetGrowCapacity(capacity));
	}
}

// Resize
//------------------------------------------------------------------------------
template<class T, class Allocator>
void Array<T, Allocator>::Resize(size_t size, size_t capacity)
{
	const size_t oldCapacity = GetCapacity();
	const size_t oldSize = GetSize();

	if (capacity != oldCapacity)
	{
		T * newMem = Reallocate(capacity);
		if (size > oldSize)
		{
			UninitializedCopy(newMem, m_Begin, oldSize);
			InPlaceConstruct(newMem + oldSize, size - oldSize);
		}
		else
		{
			UninitializedCopy(newMem, m_Begin, size);
		}
		InPlaceDestruct(m_Begin, m_End - m_Begin);
		Deallocate();
		m_Begin = newMem;
	}
	else if (size != oldSize)
	{
		if (size > oldSize)
		{
			InPlaceConstruct(m_Begin + oldSize, size - oldSize);
		}
		else
		{
			InPlaceDestruct(m_Begin + size, oldSize - size);
		}
	}
	m_End = m_Begin + size;
	m_MaxEnd = m_Begin + capacity;
}

template<class T, class Allocator>
T * Array<T, Allocator>::Allocate(size_t size)
{
	return (T *)m_AllocatorInst.Allocate(size * sizeof(T));
}

template<class T, class Allocator>
T * Array<T, Allocator>::Reallocate(size_t size)
{
	return (T *)m_AllocatorInst.Allocate(size * sizeof(T));
}

template<class T, class Allocator>
void Array<T, Allocator>::Deallocate()
{
	m_AllocatorInst.Free(m_Begin);
}


// Static functions
//------------------------------------------------------------------------------

// InPlaceConstruct
//------------------------------------------------------------------------------
template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::InPlaceConstruct(T * mem, size_t count)
{
	InPlaceConstruct(mem, count, std::is_trivially_constructible<T>());
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::InPlaceConstruct(T * mem, size_t count, const std::true_type&)
{
	ASSERT(mem);
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::InPlaceConstruct(T * mem, size_t count, const std::false_type&)
{
	ASSERT(mem);

	for (size_t idx = 0; idx < count; ++idx)
	{
		INPLACE_NEW(mem + idx) T;
	}
}

// InPlaceDestruct
//------------------------------------------------------------------------------
template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::InPlaceDestruct(T * mem, size_t count)
{
	InPlaceDestruct(mem, count, std::is_trivially_destructible<T>());
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::InPlaceDestruct(T * mem, size_t count, const std::true_type&)
{
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::InPlaceDestruct(T * mem, size_t count, const std::false_type&)
{
	for (size_t idx = 0; idx < count; ++idx)
	{
		(mem + idx)->~T();
	}
}

// UninitializedCopy
//------------------------------------------------------------------------------
template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::UninitializedCopy(T * des, const T * src, size_t count)
{
	UninitializedCopy(des, src, count, std::is_trivially_copy_assignable<T>());
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::UninitializedCopy(T * des, const T * src, size_t count, const std::true_type&)
{
	MemCopy(des, src, count * sizeof(T));
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::UninitializedCopy(T * des, const T * src, size_t count, const std::false_type&)
{
	for (size_t idx = 0; idx < count; ++idx)
	{
		INPLACE_NEW (des + idx) T(src[idx]);
	}
}

// UninitializedFill
//------------------------------------------------------------------------------
template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::UninitializedFill(T * des, const T& value, size_t count)
{
	UninitializedFill(des, value, count, std::is_trivially_copy_assignable<T>());
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::UninitializedFill(T * des, const T& value, size_t count, const std::true_type&)
{
	//MemSet(des, value, count);
	for (size_t idx = 0; idx < count; ++idx)
	{
		*(des + idx) = value;
	}
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::UninitializedFill(T * des, const T& value, size_t count, const std::false_type&)
{
	for (size_t idx = 0; idx < count; ++idx)
	{
		INPLACE_NEW(des + idx) T(value);
	}
}

// Move
//------------------------------------------------------------------------------
template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::Move(T * des, T * src, size_t count)
{
	Move(des, src, count, std::is_trivially_copy_assignable<T>());
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::Move(T * des, T * src, size_t count, const std::true_type&)
{
	MemMove(des, src, count * sizeof(T));
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::Move(T * des, T * src, size_t count, const std::false_type&)
{
	if (des <= src)
	{
		for (size_t idx = 0; idx < count; ++idx)
		{
			des[idx] = src[idx];
		}
	}
	else
	{
		for (size_t idx = count; idx > 0; --idx)
		{
			des[idx] = src[idx];
		}
	}
}

// Copy
//------------------------------------------------------------------------------
template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::Copy(T * des, T * src, size_t count)
{
	Copy(des, src, count, std::is_trivially_copy_assignable<T>());
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::Copy(T * des, T * src, size_t count, const std::true_type&)
{
	MemCopy(des, src, count * sizeof(T));
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::Copy(T * des, T * src, size_t count, const std::false_type&)
{
	for (size_t idx = 0; idx < count; ++idx)
	{
		des[idx] = src[idx];
	}
}

// Set
//------------------------------------------------------------------------------
template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::Set(T * des, const T& value, size_t count)
{
	for (size_t idx = 0; idx < count; ++idx)
	{
		des[idx] = value;
	}
}

//------------------------------------------------------------------------------