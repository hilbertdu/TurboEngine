// Array.inl
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
Array<T, Allocator>::Array(const Allocator & allocator)
	: Array()
{
	m_Allocator = allocator;
}

template<class T, class Allocator>
Array<T, Allocator>::Array(SIZET initSize)
	: Array()
{
	SetSize(initSize);
}

template<class T, class Allocator>
Array<T, Allocator>::Array(SIZET initSize, const Allocator & allocator)
	: Array()
{
	m_Allocator = allocator;
	SetSize(initSize);
}

template<class T, class Allocator>
Array<T, Allocator>::Array(const T * begin, const T * end)
	: Array()
{
	const SIZET size = (end - begin);
	SetCapacity(size);
	UninitializedCopy(m_Begin, begin, size);
	m_End = m_Begin + size;
}

template<class T, class Allocator>
Array<T, Allocator>::Array(const T * begin, const T * end, const Allocator & allocator)
	: Array()
{
	m_Allocator = allocator;
	const SIZET size = (end - begin);
	SetCapacity(size);
	UninitializedCopy(m_Begin, begin, size);
	m_End = m_Begin + size;
}

template<class T, class Allocator>
Array<T, Allocator>::Array(const std::initializer_list<T> & initList)
{
	SetCapacity(initList.size());
	UninitializedCopy(m_Begin, initList.begin(), initList.size());
	m_End = m_Begin + initList.size();
}

template<class T, class Allocator>
Array<T, Allocator>::Array(const std::initializer_list<T> & initList, const Allocator & allocator)
	: m_Allocator(allocator)
{
	SetCapacity(initList.size());
	UninitializedCopy(m_Begin, initList.begin(), initList.size());
	m_End = m_Begin + initList.size();
}

// Copy constructors
//------------------------------------------------------------------------------
template<class T, class Allocator>
Array<T, Allocator>::Array(const Array & other)
	: m_Begin(nullptr)
	, m_End(nullptr)
	, m_MaxEnd(nullptr)
{
	const SIZET size = other.GetSize();
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
	const SIZET size = other.GetSize();
	SetCapacity(other.GetCapacity());
	UninitializedCopy(m_Begin, other.m_Begin, size);
	m_End = m_Begin + size;
}

// Move copy constructors
//------------------------------------------------------------------------------
template<class T, class Allocator>
template<class>
Array<T, Allocator>::Array(Array && other)
	: m_Begin(other.m_Begin)
	, m_End(other.m_End)
	, m_MaxEnd(other.m_MaxEnd)
	, m_Allocator(std::move(other.m_Allocator))
{
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
void Array<T, Allocator>::SetCapacity(SIZET capacity)
{
	Resize(GetSize(), capacity);
}

template<class T, class Allocator>
void Array<T, Allocator>::SetSize(SIZET size)
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
void Array<T, Allocator>::Swap(Array & other)
{
	std::move(m_Begin, other.m_Begin);
	std::move(m_End, other.m_End);
	std::move(m_MaxEnd, other.m_MaxEnd);
	std::move(m_Allocator, other.m_Allocator);
}

template<class T, class Allocator>
void Array<T, Allocator>::SwapItem(SIZET index1, SIZET index2)
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
SIZET Array<T, Allocator>::FindPos(const U & obj) const
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

template<class T, class Allocator>
template<class Functor>
T * Array<T, Allocator>::FindIf(const Functor & cmp) const
{
	T * pos = m_Begin;
	T * end = m_End;
	while (pos < end)
	{
		if (cmp(*pos))
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
void Array<T, Allocator>::Append(const T & item, SIZET count)
{
	const SIZET newSize = GetSize() + count;
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
	const SIZET newSize = GetSize() + other.GetSize();
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
	const SIZET newSize = GetSize() + (end - begin);
	Grow(newSize);
	UninitializedCopy(m_End, begin, end - begin);
	m_End += (end - begin);
}

// Append
//------------------------------------------------------------------------------
template<class T, class Allocator>
bool Array<T, Allocator>::AppendUnique(const T &item)
{
	if (!Find(item))
	{
		Append(item);
		return true;
	}
	return false;
}

// FindOrAppend
//------------------------------------------------------------------------------
template<class T, class Allocator>
SIZET Array<T, Allocator>::FindOrAppend(const T & item)
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
void Array<T, Allocator>::Insert(SIZET index, const T & item, SIZET count)
{
	ASSERT(index >= 0 && index <= GetSize());

	const SIZET oldSize = GetSize();
	const SIZET newSize = oldSize + count;

	if (newSize > GetCapacity())
	{
		SIZET newCapacity = GetGrowCapacity(newSize);
		T * newMem = Reallocate(newCapacity);

		UninitializedCopy(newMem, m_Begin, index);
		UninitializedFill(newMem + index, item, count);
		UninitializedCopy(newMem + index + count, m_Begin + index, oldSize - index);
		
		InPlaceDestruct(m_Begin, m_End - m_Begin);
		Deallocate();

		m_Begin = newMem;
		m_End = m_Begin + oldSize + count;
		m_MaxEnd = m_Begin + newCapacity;
	}
	else
	{
		T * posInsert = m_Begin + index;
		const SIZET shiftCount = m_End - posInsert;
		
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
void Array<T, Allocator>::Insert(Iter iter, const T & item, SIZET count)
{
	Insert(iter - m_Begin, item, count);
}

// Pop
//------------------------------------------------------------------------------
template<class T, class Allocator>
void Array<T, Allocator>::Pop(SIZET count)
{
	Erase(m_End - count, count);
}

// PopFront
//------------------------------------------------------------------------------
template<class T, class Allocator>
void Array<T, Allocator>::PopFront(SIZET count)
{
	Erase(m_Begin, count);
}

// Erase (iter)
//------------------------------------------------------------------------------
template<class T, class Allocator>
void Array<T, Allocator>::Erase(T * const iter, SIZET count)
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
Array<T, Allocator> & Array<T, Allocator>::operator = (const Array & other)
{
	if (this != &other)
	{
		InPlaceDestruct(m_Begin, m_End - m_Begin);

		// don't shrink capacity to other capacity
		const SIZET newSize = other.GetSize();
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
		const SIZET newSize = other.GetSize();
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
template<class>
Array<T, Allocator> & Array<T, Allocator>::operator = (Array && rOther)
{
	Swap(rOther);
}

// EraseSwap
//-----------------------------------------------------------------------------
template<class T, class Allocator>
void Array<T, Allocator>::EraseSwapIndex(SIZET index, SIZET count)
{
	ASSERT(index <= GetSize());
	ASSERT(index + count <= GetSize());

	SIZET shiftStartIndex = index + count;
	SIZET trailingCount = GetSize() - shiftStartIndex;
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
SIZET Array<T, Allocator>::GetGrowCapacity(SIZET capacity)
{
	SIZET currentCapacity = m_MaxEnd - m_Begin;
	return Math::Max(capacity, currentCapacity + (currentCapacity >> 1) + 1);
}

// Grow
//------------------------------------------------------------------------------
template<class T, class Allocator>
void Array<T, Allocator>::Grow(SIZET capacity)
{
	if (capacity > GetCapacity())
	{
		Resize(GetSize(), GetGrowCapacity(capacity));
	}
}

// Resize
//------------------------------------------------------------------------------
template<class T, class Allocator>
void Array<T, Allocator>::Resize(SIZET size, SIZET capacity)
{
	const SIZET oldCapacity = GetCapacity();
	const SIZET oldSize = GetSize();

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
T * Array<T, Allocator>::Allocate(SIZET size)
{
	return m_Allocator.AllocateT<T>(size);
}

template<class T, class Allocator>
T * Array<T, Allocator>::Reallocate(SIZET size)
{
	return m_Allocator.AllocateT<T>(size);
}

template<class T, class Allocator>
void Array<T, Allocator>::Deallocate()
{
	if (m_Begin)
	{
		m_Allocator.FreeT(m_Begin);
	}
}


// Static functions
//------------------------------------------------------------------------------

// InPlaceConstruct
//------------------------------------------------------------------------------
template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::InPlaceConstruct(T * mem, SIZET count)
{
	InPlaceConstruct(mem, count, std::is_trivially_constructible<T>());
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::InPlaceConstruct(T * mem, SIZET count, const std::true_type&)
{
	ASSERT(mem);
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::InPlaceConstruct(T * mem, SIZET count, const std::false_type&)
{
	ASSERT(mem);

	for (SIZET idx = 0; idx < count; ++idx)
	{
		new(mem + idx) T;
	}
}

// InPlaceDestruct
//------------------------------------------------------------------------------
template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::InPlaceDestruct(T * mem, SIZET count)
{
	InPlaceDestruct(mem, count, std::is_trivially_destructible<T>());
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::InPlaceDestruct(T * mem, SIZET count, const std::true_type&)
{
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::InPlaceDestruct(T * mem, SIZET count, const std::false_type&)
{
	for (SIZET idx = 0; idx < count; ++idx)
	{
		(mem + idx)->~T();
	}
}

// UninitializedCopy
//------------------------------------------------------------------------------
template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::UninitializedCopy(T * des, const T * src, SIZET count)
{
	UninitializedCopy(des, src, count, std::is_trivially_copy_assignable<T>());
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::UninitializedCopy(T * des, const T * src, SIZET count, const std::true_type&)
{
	MemCopy(des, src, count * sizeof(T));
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::UninitializedCopy(T * des, const T * src, SIZET count, const std::false_type&)
{
	for (SIZET idx = 0; idx < count; ++idx)
	{
		INPLACE_NEW (des + idx) T(src[idx]);
	}
}

// UninitializedFill
//------------------------------------------------------------------------------
template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::UninitializedFill(T * des, const T& value, SIZET count)
{
	UninitializedFill(des, value, count, std::is_trivially_copy_assignable<T>());
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::UninitializedFill(T * des, const T& value, SIZET count, const std::true_type&)
{
	for (SIZET idx = 0; idx < count; ++idx)
	{
		*(des + idx) = value;
	}
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::UninitializedFill(T * des, const T& value, SIZET count, const std::false_type&)
{
	for (SIZET idx = 0; idx < count; ++idx)
	{
		INPLACE_NEW(des + idx) T(value);
	}
}

// Move
//------------------------------------------------------------------------------
template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::Move(T * des, T * src, SIZET count)
{
	Move(des, src, count, std::is_trivially_copy_assignable<T>());
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::Move(T * des, T * src, SIZET count, const std::true_type&)
{
	MemMove(des, src, count * sizeof(T));
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::Move(T * des, T * src, SIZET count, const std::false_type&)
{
	if (des <= src)
	{
		for (SIZET idx = 0; idx < count; ++idx)
		{
			des[idx] = src[idx];
		}
	}
	else
	{
		for (SIZET idx = count; idx > 0; --idx)
		{
			des[idx] = src[idx];
		}
	}
}

// Copy
//------------------------------------------------------------------------------
template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::Copy(T * des, T * src, SIZET count)
{
	Copy(des, src, count, std::is_trivially_copy_assignable<T>());
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::Copy(T * des, T * src, SIZET count, const std::true_type&)
{
	MemCopy(des, src, count * sizeof(T));
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::Copy(T * des, T * src, SIZET count, const std::false_type&)
{
	for (SIZET idx = 0; idx < count; ++idx)
	{
		des[idx] = src[idx];
	}
}

// Set
//------------------------------------------------------------------------------
template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::Set(T * des, const T& value, SIZET count)
{
	for (SIZET idx = 0; idx < count; ++idx)
	{
		des[idx] = value;
	}
}

//------------------------------------------------------------------------------