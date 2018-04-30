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
Array<T, Allocator>::Array(SIZET initSize, const T & element)
	: Array()
{
	SetCapacity(initSize);
	UninitializedFill(m_Begin, element, initSize);
	m_End = m_Begin + initSize;
}

template<class T, class Allocator>
Array<T, Allocator>::Array(std::initializer_list<T> initList)
{
	SetCapacity(initList.size());
	UninitializedCopy(m_Begin, initList.begin(), initList.size());
	m_End = m_Begin + initList.size();
}

template<class T, class Allocator>
Array<T, Allocator>::Array(std::initializer_list<T> initList, const Allocator & allocator)
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
Array<T, Allocator>::Array(Array && other)
	: m_Begin(other.m_Begin)
	, m_End(other.m_End)
	, m_MaxEnd(other.m_MaxEnd)
	, m_Allocator(std::move(other.m_Allocator))
{
	other.m_Begin = nullptr;
	other.m_End = nullptr;
	other.m_MaxEnd = nullptr;
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
	std::swap(m_Begin, other.m_Begin);
	std::swap(m_End, other.m_End);
	std::swap(m_MaxEnd, other.m_MaxEnd);
	std::swap(m_Allocator, other.m_Allocator);
}

template<class T, class Allocator>
void Array<T, Allocator>::SwapItem(Iter iter1, Iter iter2)
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
void Array<T, Allocator>::Append(T && item)
{
	Grow(GetSize() + 1);
	InPlaceConstruct(m_End, 1, std::move(item));
	m_End += 1;
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
typename Array<T, Allocator>::Iter Array<T, Allocator>::Insert(Iter iter, const T & item, SIZET count)
{
	ASSERT(index >= 0 && index <= GetSize());

	const SIZET index = iter - m_Begin;
	const SIZET oldSize = GetSize();
	const SIZET newSize = oldSize + count;

	if (newSize > GetCapacity())
	{
		SIZET newCapacity = GetGrowCapacity(newSize);
		T * newMem = Reallocate(newCapacity);

		UninitializedMove(newMem, m_Begin, index);
		UninitializedFill(newMem + index, item, count);
		UninitializedMove(newMem + index + count, m_Begin + index, oldSize - index);
		
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
			UninitializedMove(posInsert + count, posInsert, shiftCount);
			UninitializedFill(m_End, item, count - shiftCount);
			Set(posInsert, item, shiftCount);
		}
		else
		{
			UninitializedMove(m_End, m_End - count, count);
			Move(posInsert + count, posInsert + index, shiftCount - count);
			Set(posInsert, item, count);
		}

		m_End = m_Begin + oldSize + count;
	}
	return m_Begin + count;
}

// Insert
//------------------------------------------------------------------------------
template<class T, class Allocator>
typename Array<T, Allocator>::Iter Array<T, Allocator>::Insert(Iter iter, T && item)
{
	return EmplaceInsert(iter, std::move(item));
}

template<class T, class Allocator>
template<class... TArgs>
typename Array<T, Allocator>::Iter Array<T, Allocator>::EmplaceAppend(TArgs&&... args)
{
	Grow(GetSize() + 1);
	InPlaceConstruct(m_End, 1, std::forward<TArgs>(args)...);
	m_End += 1;
	return m_End - 1;
}

template<class T, class Allocator>
template<class... TArgs>
typename Array<T, Allocator>::Iter Array<T, Allocator>::EmplaceInsert(Iter iter, TArgs&&... args)
{
	ASSERT(iter >= m_Begin && iter <= m_End);

	const SIZET oldSize = GetSize();
	const SIZET newSize = oldSize + 1;
	SIZET index = iter - m_Begin;

	if (newSize > GetCapacity())
	{
		SIZET newCapacity = GetGrowCapacity(newSize);
		T * newMem = Reallocate(newCapacity);

		UninitializedMove(newMem, m_Begin, index);
		InPlaceConstruct(newMem + index, 1, std::forward<TArgs>(args)...);
		UninitializedMove(newMem + index + 1, m_Begin + index, oldSize - index);

		InPlaceDestruct(m_Begin, m_End - m_Begin);
		Deallocate();

		m_Begin = newMem;
		m_End = m_Begin + oldSize + 1;
		m_MaxEnd = m_Begin + newCapacity;
	}
	else
	{
		if (iter != m_End)
		{
			UninitializedMove(m_End, m_End - 1, 1);
			Move(iter + 1, iter + index, m_End - iter - 1);
		}
		InPlaceConstruct(iter, 1, std::forward<TArgs>(args)...);

		m_End = m_Begin + oldSize + 1;
	}
	return iter;
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
Array<T, Allocator> & Array<T, Allocator>::operator = (Array && rOther)
{
	Swap(rOther);
	return *this;
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
	//return Math::Max(capacity, (currentCapacity << 1));
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

	if (capacity == 0)
	{
		InPlaceDestruct(m_Begin, m_End - m_Begin);
		Deallocate();
		m_Begin = nullptr;
		m_End = nullptr;
		m_MaxEnd = nullptr;
		return;
	}
	
	if (capacity != oldCapacity)
	{
		T * newMem = Reallocate(capacity);
		if (size > oldSize)
		{
			UninitializedMove(newMem, m_Begin, oldSize);
			InPlaceConstruct(newMem + oldSize, size - oldSize);
		}
		else
		{
			UninitializedMove(newMem, m_Begin, size);
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
template<class... TArgs>
/*static*/ void Array<T, Allocator>::InPlaceConstruct(T * mem, SIZET count, TArgs&&... args)
{
	_InPlaceConstruct(mem, count, std::false_type(), std::forward<TArgs>(args)...);
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::InPlaceConstruct(T * mem, SIZET count)
{
	_InPlaceConstruct(mem, count, std::is_trivially_constructible<T>());
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::_InPlaceConstruct(T * mem, SIZET count, const std::true_type&)
{
	ASSERT(mem);
}

template<class T, class Allocator>
template<class... TArgs>
/*static*/ void Array<T, Allocator>::_InPlaceConstruct(T * mem, SIZET count, const std::false_type&, TArgs&&... args)
{
	ASSERT(mem);

	for (SIZET idx = 0; idx < count; ++idx)
	{
		INPLACE_NEW(mem + idx) T(std::forward<TArgs>(args)...);
	}
}

// InPlaceDestruct
//------------------------------------------------------------------------------
template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::InPlaceDestruct(T * mem, SIZET count)
{
	_InPlaceDestruct(mem, count, std::is_trivially_destructible<T>());
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::_InPlaceDestruct(T * mem, SIZET count, const std::true_type&)
{
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::_InPlaceDestruct(T * mem, SIZET count, const std::false_type&)
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
	_UninitializedCopy(des, src, count, std::is_trivially_copy_assignable<T>());
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::_UninitializedCopy(T * des, const T * src, SIZET count, const std::true_type&)
{
	MemCopy(des, src, count * sizeof(T));
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::_UninitializedCopy(T * des, const T * src, SIZET count, const std::false_type&)
{
	for (SIZET idx = 0; idx < count; ++idx)
	{
		INPLACE_NEW (des + idx) T(src[idx]);
	}
}

// UninitializedMove
//------------------------------------------------------------------------------
template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::UninitializedMove(T * des, T * src, SIZET count)
{
	_UninitializedMove(des, src, count, std::is_trivially_move_assignable<T>());
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::_UninitializedMove(T * des, T * src, SIZET count, const std::true_type&)
{
	MemCopy(des, src, count * sizeof(T));
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::_UninitializedMove(T * des, T * src, SIZET count, const std::false_type&)
{
	for (SIZET idx = 0; idx < count; ++idx)
	{
		INPLACE_NEW(des + idx) T(std::move(src[idx]));
	}
}

// UninitializedFill
//------------------------------------------------------------------------------
template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::UninitializedFill(T * des, const T& value, SIZET count)
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
	_Move(des, src, count, std::is_trivially_move_assignable<T>());
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::_Move(T * des, T * src, SIZET count, const std::true_type&)
{
	MemMove(des, src, count * sizeof(T));
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::_Move(T * des, T * src, SIZET count, const std::false_type&)
{
	if (des <= src)
	{
		for (SIZET idx = 0; idx < count; ++idx)
		{
			des[idx] = std::move(src[idx]);
		}
	}
	else
	{
		for (SIZET idx = count; idx > 0; --idx)
		{
			des[idx] = std::move(src[idx]);
		}
	}
}

// Copy
//------------------------------------------------------------------------------
template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::Copy(T * des, T * src, SIZET count)
{
	_Copy(des, src, count, std::is_trivially_copy_assignable<T>());
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::_Copy(T * des, T * src, SIZET count, const std::true_type&)
{
	MemCopy(des, src, count * sizeof(T));
}

template<class T, class Allocator>
/*static*/ void Array<T, Allocator>::_Copy(T * des, const T * src, SIZET count, const std::false_type&)
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