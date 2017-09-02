// List implemention
//------------------------------------------------------------------------------


// ListIterator
//------------------------------------------------------------------------------
template<class List, bool Const>
ListIterator<List, Const>::ListIterator(ListNodeBase* pNode)
	: m_Pos(static_cast<ListNodePtr>(pNode))
{
}

template<class List, bool Const>
ListIterator<List, Const>::ListIterator(const ListIterator<List, !Const>& rOther)
	: m_Pos(rOther.m_Pos)
{
}

template<class List, bool Const>
ListIterator<List, Const>& ListIterator<List, Const>::operator=(const ListIterator<List, !Const>& rOther)
{
	m_Pos = rOther.m_Pos;
	return *this;
}

template<class List, bool Const>
typename const ListIterator<List, Const>::ValueType& ListIterator<List, Const>::operator*() const
{
	ASSERT(m_Pos);
	return m_Pos->m_Value;
}

template<class List, bool Const>
typename const ListIterator<List, Const>::ValueType* ListIterator<List, Const>::operator->() const
{
	ASSERT(m_Pos);
	return &m_Pos->m_Value;
}

template<class List, bool Const>
template<class>
typename ListIterator<List, Const>::ValueType& ListIterator<List, Const>::operator*()
{
	ASSERT(m_Pos);
	return m_Pos->m_Value;
}

template<class List, bool Const>
template<class>
typename ListIterator<List, Const>::ValueType* ListIterator<List, Const>::operator->()
{
	ASSERT(m_Pos);
	return &m_Pos->m_Value;
}

template<class List, bool Const>
ListIterator<List, Const>& ListIterator<List, Const>::operator++()
{
	ASSERT(m_Pos);
	m_Pos = (ListNodePtr)m_Pos->m_Next;
	return *this;
}

template<class List, bool Const>
ListIterator<List, Const> ListIterator<List, Const>::operator++(int)
{
	ASSERT(m_Pos);
	ListIterator iter = *this;
	++(*this);
	return iter;
}

template<class List, bool Const>
ListIterator<List, Const>& ListIterator<List, Const>::operator--()
{
	ASSERT(m_Pos);
	m_Pos = (ListNodePtr)m_Pos->m_Prev;
	return *this;
}

template<class List, bool Const>
ListIterator<List, Const> ListIterator<List, Const>::operator--(int)
{
	ASSERT(m_Pos);
	ListIterator iter = *this;
	--(*this);
	return iter;
}

template<class List, bool Const>
bool ListIterator<List, Const>::operator==(const ListIterator& rOther) const
{
	return m_Pos == rOther.m_Pos;
}

template<class List, bool Const>
bool ListIterator<List, Const>::operator!=(const ListIterator& rOther) const
{
	return m_Pos != rOther.m_Pos;
}

template<class List, bool Const>
bool ListIterator<List, Const>::operator<(const ListIterator& rOther) const
{
}

template<class List, bool Const>
bool ListIterator<List, Const>::operator>(const ListIterator& rOther) const
{}

template<class List, bool Const>
bool ListIterator<List, Const>::operator<=(const ListIterator& rOther) const
{}

template<class List, bool Const>
bool ListIterator<List, Const>::operator>=(const ListIterator& rOther) const
{}


// Constructors
//------------------------------------------------------------------------------
template<class T, class Allocator>
LinkedList<T, Allocator>::LinkedList(const Allocator & allocator)
	: m_Allocator(allocator)
	, m_Head(AllocateNode())
	, m_Size(0)
{
	m_Head->m_Next = m_Head;
	m_Head->m_Prev = m_Head;
}

template<class T, class Allocator>
LinkedList<T, Allocator>::LinkedList(SIZET initSize, const Allocator & allocator)
	: LinkedList(allocator)
{
	Insert(Begin(), initSize, T());
}

template<class T, class Allocator>
template<class OtherAllocator>
LinkedList<T, Allocator>::LinkedList(const LinkedList<T, OtherAllocator> & other)
	: LinkedList(other.m_Allocator)
{
	Insert(Begin(), other.Begin(), other.End());
}

template<class T, class Allocator>
template<class OtherAllocator>
LinkedList<T, Allocator>::LinkedList(LinkedList<T, OtherAllocator> && other)
	: m_Allocator(std::move(other.m_Allocator))
	, m_Head(std::move(other.m_Head))
	, m_Size(other.m_Size)
{
	m_Head->m_Next = m_Head;
	m_Head->m_Prev = m_Head;
	other.m_Head = nullptr;
	other.m_Size = 0;
}

// Destructors
//------------------------------------------------------------------------------
template<class T, class Allocator>
LinkedList<T, Allocator>::~LinkedList()
{
	Clear();
	DeallocateNode(m_Head);
}

// Copy constructors
//------------------------------------------------------------------------------
template<class T, class Allocator>
LinkedList<T, Allocator>::LinkedList(const MyType & other)
	: LinkedList(other.m_Allocator)
{
	Insert(Begin(), other.Begin(), other.End());
}

template<class T, class Allocator>
LinkedList<T, Allocator>::LinkedList(MyType && other)
	: m_Allocator(std::move(other.m_Allocator))
	, m_Head(std::move(other.m_Head))
	, m_Size(other.m_Size)
{
	other.m_Head = nullptr;
	other.m_Size = 0;
}

// Operator =
//------------------------------------------------------------------------------
template<class T, class Allocator>
LinkedList<T, Allocator>& LinkedList<T, Allocator>::operator = (MyType other)
{
	LinkedList(other).Swap(*this);
	return *this;
}

// Swap
//------------------------------------------------------------------------------
template<class T, class Allocator>
void LinkedList<T, Allocator>::Swap(MyType& other)
{
	std::swap(m_Head, other.m_Head);
	std::swap(m_Size, other.m_Size);
	std::swap(m_Allocator, other.m_Allocator);
}

// Insert
//------------------------------------------------------------------------------
template<class T, class Allocator>
typename LinkedList<T, Allocator>::Iter LinkedList<T, Allocator>::Insert(Iter pos, const T& value)
{
	_Insert(pos, value);
	return --pos;
}

template<class T, class Allocator>
typename LinkedList<T, Allocator>::Iter LinkedList<T, Allocator>::Insert(Iter pos, T&& value)
{
	_Insert(pos, value);
	return --pos;
}

// Erase
//------------------------------------------------------------------------------
template<class T, class Allocator>
typename LinkedList<T, Allocator>::Iter LinkedList<T, Allocator>::Erase(ConstIter pos)
{
	ASSERT(pos.m_Pos != m_Head);
	ListNodePtr pCurr = pos.GetNode();
	ListNodePtr pNext = (ListNodePtr)pCurr->m_Next;

	pCurr->m_Prev->m_Next = pNext;
	pNext->m_Prev = pCurr->m_Prev;
	pCurr->m_Value.~T();
	DeallocateNode(pCurr);
	--m_Size;
	return Iter(pNext);
}

template<class T, class Allocator>
typename LinkedList<T, Allocator>::Iter LinkedList<T, Allocator>::Erase(ConstIter first, ConstIter last)
{
	while (first != last)
	{
		first = Erase(first);
	}
	return Iter(last);
}

// Emplace
//------------------------------------------------------------------------------
template<class T, class Allocator>
template<class... TArgs>
void LinkedList<T, Allocator>::EmplaceFront(TArgs&&... args)
{
	Emplace(Begin(), std::forward<TArgs>(args)...);
}

template<class T, class Allocator>
template<class... TArgs>
void LinkedList<T, Allocator>::EmplaceBack(TArgs&&... args)
{
	Emplace(End(), std::forward<TArgs>(args)...);
}

template<class T, class Allocator>
template<class... TArgs>
typename LinkedList<T, Allocator>::Iter LinkedList<T, Allocator>::Emplace(ConstIter pos, TArgs&&... args)
{
	_Insert(pos, std::forward<TArgs>(args)...);
	return (pos--);
}

// Insert range
//------------------------------------------------------------------------------
template<class T, class Allocator>
typename LinkedList<T, Allocator>::Iter LinkedList<T, Allocator>::Insert(Iter pos, SIZET size, const T& value)
{
	while (size-- > 0)
	{
		_Insert(pos, value);
		--pos;
	}
	return pos;
}

template<class T, class Allocator>
template<class InputIter>
typename LinkedList<T, Allocator>::Iter LinkedList<T, Allocator>::Insert(Iter pos, InputIter first, InputIter last)
{
	Iter start = Begin();
	while (first++ != last)
	{
		start = Insert(pos, *first);
	}
	return start;
}

// Resize
//------------------------------------------------------------------------------
template<class T, class Allocator>
void LinkedList<T, Allocator>::Resize(SIZET newSize)
{
	if (m_Size > newSize)
	{
		while (newSize++ < m_Size)
		{
			Erase(Last());
		}
	}
	else
	{
		while (newSize-- > m_Size)
		{
			PushBack(ValueType());
		}
	}
}

// Clear
//------------------------------------------------------------------------------
template<class T, class Allocator>
void LinkedList<T, Allocator>::Clear()
{
	Erase(Begin(), End());
}

// _Insert
//------------------------------------------------------------------------------
template<class T, class Allocator>
template<class U>
void LinkedList<T, Allocator>::_Insert(Iter pos, U&& item)
{
	ListNodePtr pNext = pos.GetNode();
	ListNodePtr pNode = AllocateNode();
	INPLACE_NEW(&pNode->m_Value) T(std::forward<U>(item));

	pNode->m_Prev = pNext->m_Prev;
	pNode->m_Prev->m_Next = pNode;
	pNode->m_Next = pNext;
	pNext->m_Prev = pNode;
	++m_Size;
}

// AllocateNode
//------------------------------------------------------------------------------
template<class T, class Allocator>
typename LinkedList<T, Allocator>::ListNodePtr LinkedList<T, Allocator>::AllocateNode()
{
	return m_Allocator.AllocateT<ListNodeType>(1);
}

// DeallocateNode
//------------------------------------------------------------------------------
template<class T, class Allocator>
void LinkedList<T, Allocator>::DeallocateNode(ListNodePtr pNode)
{
	m_Allocator.FreeT<ListNodeType>(pNode);
}