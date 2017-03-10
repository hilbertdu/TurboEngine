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

template
struct


// HashTableIterator
//------------------------------------------------------------------------------
template<class List, bool Const = true>
class LinkedListIterator
{
public:
	typedef typename List::ValueType		ValueType;
	typedef typename List::AllocatorType	AllocatorType;

	explicit LinkedListIterator() = default;
	LinkedListIterator(const LinkedListIterator& rOther) = default;
	LinkedListIterator& operator=(const LinkedListIterator& rOther) = default;

	LinkedListIterator(const LinkedListIterator<Table, !Const>& rOther);
	LinkedListIterator& operator=(const LinkedListIterator<Table, !Const>& rOther);

	const ValueType& operator*() const;
	const ValueType* operator->() const;

	template<class = std::enable_if<!Const>::type>
	ValueType& operator*();

	template<class = std::enable_if<!Const>::type>
	ValueType* operator->();

	LinkedListIterator& operator++();
	LinkedListIterator  operator++(int);
	LinkedListIterator& operator--();
	LinkedListIterator  operator--(int);

	bool operator==(const LinkedListIterator& rOther) const;
	bool operator!=(const LinkedListIterator& rOther) const;
	bool operator<(const LinkedListIterator& rOther) const;
	bool operator>(const LinkedListIterator& rOther) const;
	bool operator<=(const LinkedListIterator& rOther) const;
	bool operator>=(const LinkedListIterator& rOther) const;

protected:
	List* m_List;


	explicit HashTableIterator(Table* pTable, SIZET bucketIndex, SIZET elementIndex);

	friend class HashTable<ValueType, KeyType, HasherType, ExtractKeyType, KeyEqualType, AllocatorType>;
	friend class HashTableIterator<Table, !Const>;
};


template<class T, class Allocator = DefaultAllocator>
class LinkedList
{
public:


	explicit LinkedList(const Allocator & allocator = Allocator());
	explicit LinkedList(SIZET initSize, const Allocator & allocator = Allocator());


	Array(const Array<T> & other);
	template<class OtherAllocator>
	Array(const Array<T, OtherAllocator> & other);

	Array(Array<T> && other);
	template<class OtherAllocator>
	Array(Array<T, OtherAllocator> && other);


};


//------------------------------------------------------------------------------
#endif FOUNDATION_CONTAINER_LIST_H