// List.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_CONTAINER_LIST_H
#define FOUNDATION_CONTAINER_LIST_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/Platform/Misc.h"
#include "Foundation/Memory/Allocator.h"
#include "Foundation/Memory/Mem.h"
#include <type_traits>


// Forward declaration
//------------------------------------------------------------------------------
template<class T, class Allocator>
class LinkedList;


// ListNode
//------------------------------------------------------------------------------
struct ListNodeBase
{
	ListNodeBase * m_Prev;
	ListNodeBase * m_Next;
};

template<class T>
struct ListNode : public ListNodeBase
{
	T m_Value;
};


// ListIterator
//------------------------------------------------------------------------------
template<class List, bool Const = true>
class ListIterator
{
public:
	typedef typename List::ValueType		ValueType;
	typedef typename List::AllocatorType	AllocatorType;
	typedef typename List::ListNodeType		ListNodeType;
	typedef typename List::ListNodePtr		ListNodePtr;

	explicit ListIterator() = default;
	ListIterator(const ListIterator& rOther) = default;
	ListIterator(const ListIterator<List, !Const>& rOther);

	ListIterator& operator=(const ListIterator& rOther) = default;
	ListIterator& operator=(const ListIterator<List, !Const>& rOther);

	inline ListNodePtr GetNode() { return m_Pos; }

	const ValueType& operator*() const;
	const ValueType* operator->() const;

	template<class = std::enable_if<!Const>::type>
	ValueType& operator*();

	template<class = std::enable_if<!Const>::type>
	ValueType* operator->();

	ListIterator& operator++();
	ListIterator  operator++(int);
	ListIterator& operator--();
	ListIterator  operator--(int);

	bool operator==(const ListIterator& rOther) const;
	bool operator!=(const ListIterator& rOther) const;
	bool operator<(const ListIterator& rOther) const;
	bool operator>(const ListIterator& rOther) const;
	bool operator<=(const ListIterator& rOther) const;
	bool operator>=(const ListIterator& rOther) const;

protected:
	ListNodePtr m_Pos;

	explicit ListIterator(ListNodeBase* pNode);

	friend class LinkedList<ValueType, AllocatorType>;
	friend class ListIterator<List, !Const>;
};


// LinkedList
//------------------------------------------------------------------------------
template<class T, class Allocator = DefaultAllocator>
class LinkedList
{
public:
	typedef T				ValueType;
	typedef Allocator		AllocatorType;
	typedef ListNode<T>		ListNodeType;
	typedef ListNodeType*	ListNodePtr;

	typedef LinkedList<T, Allocator> MyType;

	typedef ListIterator<MyType, false> Iter;
	typedef ListIterator<MyType, true>  ConstIter;

	explicit LinkedList(const Allocator & allocator = Allocator());
	explicit LinkedList(SIZET initSize, const Allocator & allocator = Allocator());

	~LinkedList();

	template<class OtherAllocator>
	LinkedList(const LinkedList<T, OtherAllocator> & other);
	template<class OtherAllocator>
	LinkedList(LinkedList<T, OtherAllocator> && other);

	LinkedList(const MyType & other);
	LinkedList(MyType && other);

	// operator
	LinkedList& operator = (MyType other);

	// Swap
	void Swap(MyType& other);

	// Iterator
	FORCE_INLINE Iter      Begin() { return Iter(m_Head->m_Next); }
	FORCE_INLINE ConstIter Begin() const { return ConstIter(m_Head->m_Next); }
	FORCE_INLINE Iter      End() { return Iter(m_Head); }
	FORCE_INLINE ConstIter End() const { return ConstIter(m_Head); }
	FORCE_INLINE Iter	   Last() { return Iter(m_Head->m_Prev); }
	FORCE_INLINE ConstIter Last() const { return ConstIter(m_Head->m_Prev); }

	FORCE_INLINE bool  IsEmpty() const { return m_Size == 0; }
	FORCE_INLINE SIZET GetSize() const { return m_Size; }
	
	// Insert/Remove
	FORCE_INLINE void PushFront(const T& value) { _Insert(m_Head->m_Next, value); }
	FORCE_INLINE void PushFront(T&& value) { _Insert(m_Head->m_Next, std::move(value)); }
	FORCE_INLINE void PopFront() { _Erase((ListNodePtr)m_Head->m_Next); }

	FORCE_INLINE void PushBack(const T& value) { _Insert(m_Head, value); }
	FORCE_INLINE void PushBack(T&& value) { _Insert(m_Head, std::move(value)); }
	FORCE_INLINE void PopBack() { _Erase((ListNodePtr)m_Head->m_Prev); }

	FORCE_INLINE Iter Insert(Iter& pos, const T& value) { _Insert(pos.GetNode(), value); return --pos; }
	FORCE_INLINE Iter Insert(Iter& pos, T&& value) { _Insert(pos.GetNode(), std::move(value)); return --pos; }
	FORCE_INLINE Iter Erase(Iter& pos) { return _Erase(pos.GetNode()); }
	FORCE_INLINE Iter Erase(ConstIter& pos) { return _Erase(pos.GetNode()); }
	FORCE_INLINE Iter Erase(Iter& first, Iter& last) { while (first != last) { first = Erase(first); } return Iter(last); }
	FORCE_INLINE Iter Erase(ConstIter& first, ConstIter& last) { while (first != last) { first = Erase(first); } return Iter(last); }

	FORCE_INLINE void Clear() { Erase(Begin(), End()); }

	template<class... TArgs>
	FORCE_INLINE void EmplaceFront(TArgs&&... args) { Emplace(Begin(), std::forward<TArgs>(args)...); }
	template<class... TArgs>
	FORCE_INLINE void EmplaceBack(TArgs&&... args) { Emplace(End(), std::forward<TArgs>(args)...); }
	template<class... TArgs>
	FORCE_INLINE Iter Emplace(Iter& pos, TArgs&&... args);

	FORCE_INLINE Iter Insert(Iter& pos, SIZET size, const T& value);
	template<class InputIter>
	FORCE_INLINE Iter Insert(Iter& pos, InputIter& first, InputIter& last);

	void Resize(SIZET newSize);

private:
	template<class U>
	void _Insert(ListNodePtr pos, U&& item);
	Iter _Erase(ListNodePtr pos);

	FORCE_INLINE ListNodePtr AllocateNode();
	FORCE_INLINE void		 DeallocateNode(ListNodePtr pNode);

	Allocator	m_Allocator;
	ListNodePtr m_Head;
	SIZET		m_Size;
};


#include "Foundation/Container/List.inl"

//------------------------------------------------------------------------------
#endif FOUNDATION_CONTAINER_LIST_H