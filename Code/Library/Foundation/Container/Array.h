// Array.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_CONTAINER_ARRAY_H
#define FOUNDATION_CONTAINER_ARRAY_H

// Include
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/Platform/Misc.h"
#include "Foundation/Memory/Allocator.h"
#include "Foundation/Memory/Mem.h"
#include <type_traits>

//// ArrayIterator
////------------------------------------------------------------------------------
//template<typename T>
//class ArrayIterator
//{
//public:
//	explicit ArrayIterator(const T* element);
//
//	T& operator* () const;
//	T* operator-> () const;
//
//	ArrayIterator& operator++ ();
//	ArrayIterator& operator-- ();
//	ArrayIterator  operator++ (int);
//	ArrayIterator  operator-- (int);
//	ArrayIterator& operator+= (int32 offset);
//	ArrayIterator& operator-= (int32 offset);
//
//	ArrayIterator  operator+ (int32 offset);
//	ArrayIterator  operator- (int32 offset);
//
//	bool operator==(const ArrayIterator& other) const;
//	bool operator!=(const ArrayIterator& other) const;
//	bool operator<(const ArrayIterator& other) const;
//	bool operator>(const ArrayIterator& other) const;
//	bool operator<=(const ArrayIterator& other) const;
//	bool operator>=(const ArrayIterator& other) const;
//
//private:
//	T* m_Pointer;
//};

// Array
//------------------------------------------------------------------------------
template<class T, class Allocator = DefaultAllocator>
class Array
{
public:
	//typedef ArrayIterator<T>		Iter;
	//typedef ArrayIterator<const T>	ConstIter;
	typedef	T *			Iter;
	typedef const T *	ConstIter;

	explicit Array();
	explicit Array(const T * begin, const T * end);
	explicit Array(size_t initSize);

	Array(const Array<T, Allocator> & other);
	template<class OtherAllocator>
	Array(const Array<T, OtherAllocator> & other);

	~Array();

	// Iterators
	FORCE_INLINE Iter      Begin() { return m_Begin; }
	FORCE_INLINE ConstIter Begin() const { return m_Begin; }
	FORCE_INLINE Iter      End() { return m_End; }
	FORCE_INLINE ConstIter End() const { return m_End; }
	FORCE_INLINE T &		operator [] (size_t index) { ASSERT(index < GetSize()); return m_Begin[index]; }
	FORCE_INLINE const T &	operator [] (size_t index) const { ASSERT(index < GetSize()); return m_Begin[index]; }
	FORCE_INLINE T &		Top() { ASSERT(m_Begin < m_End); return m_End[-1]; }
	FORCE_INLINE const T &	Top() const { ASSERT(m_Begin < m_End); return m_End[-1]; }

	// Capacity and size
	void SetCapacity(size_t capacity);
	void SetSize(size_t size);
	void Clear();
	void Swap(Array<T> & other);
	void SwapItem(size_t index1, size_t index2);
	void SwapItem(Iter& iter1, Iter& iter2);

	// Sort
	void Sort() { ShellSort(m_Begin, m_End, AscendingCompare()); }
	void SortDeref() { ShellSort(m_Begin, m_End, AscendingCompareDeref()); }
	template<class COMPARER>
	void Sort(const COMPARER & comp) { ShellSort(m_Begin, m_End, comp); }
	void Sort(bool(*comp)(const T &, const T &)) { ShellSort(m_Begin, m_End, comp); }

	// Find
	template<class U>
	T * Find(const U & obj) const;
	template<class U>
	T * FindDeref(const U & obj) const;
	template<class U>
	T * Find(const U & obj, bool(* equal)(const T &, const U &)) const;
	template<class U>
	size_t FindPos(const U & obj) const;

	// Add / Remove
	void Append(const T & item, size_t count = 1);
	template<class U>
	void Append(const Array<U> & other);
	template <class U>
	void Append(const U * begin, const U * end);

	void Insert(size_t index, const T & item, size_t count = 1);
	void Insert(Iter iter, const T & item, size_t count = 1);

	void Pop(size_t count = 1);
	void PopFront(size_t count = 1);
	void Erase(T * const iter, size_t count = 1);
	//void Erase(const Iter & iter, size_t count = 1) { Erase(*iter, count); }
	void EraseIndex(size_t index) { Erase(m_Begin + index); }

	Array & operator = (const Array<T> & other);
	template<typename OtherAllocator>
	Array & operator = (const Array<T, OtherAllocator> & other);

	// Query
	FORCE_INLINE size_t	GetCapacity() const { return (m_MaxEnd - m_Begin); }
	FORCE_INLINE size_t	GetSize() const { return (m_End - m_Begin); }
	FORCE_INLINE bool	IsEmpty() const { return (m_Begin == m_End); }

	// Quick operation to be implement
	void EraseSwap(T * const iter, size_t count = 1);
	//void EraseSwap(const Iter iter, size_t count = 1) { EraseSwap(*iter, count); }
	void EraseSwapIndex(size_t index, size_t count = 1) { EraseSwap(m_Begin + index, count); }

	// Optimization
	void Shrink();

	// Static helper
	static void InPlaceConstruct(T * mem, size_t count);
	static void InPlaceDestruct(T * mem, size_t count);
	static void UninitializedCopy(T * des, const T * src, size_t count);
	static void UninitializedFill(T * des, const T& value, size_t count);
	static void Move(T * des, T * src, size_t count);
	static void Copy(T * des, T * src, size_t count);
	static void Set(T * des, const T& value, size_t count);

private:
	void   Resize(size_t size, size_t capacity);
	void   Grow(size_t capacity);
	size_t GetGrowCapacity(size_t capacity);

	T*   Allocate(size_t size);
	T*   Reallocate(size_t size);
	void Deallocate();

	static void InPlaceConstruct(T * mem, size_t count, const std::true_type&);
	static void InPlaceConstruct(T * mem, size_t count, const std::false_type&);
	static void InPlaceDestruct(T * mem, size_t count, const std::true_type&);
	static void InPlaceDestruct(T * mem, size_t count, const std::false_type&);
	static void UninitializedCopy(T * des, const T * src, size_t count, const std::true_type&);
	static void UninitializedCopy(T * des, const T * src, size_t count, const std::false_type&);
	static void UninitializedFill(T * des, const T& value, size_t count, const std::true_type&);
	static void UninitializedFill(T * des, const T& value, size_t count, const std::false_type&);
	static void Move(T * des, T * src, size_t count, const std::true_type&);
	static void Move(T * des, T * src, size_t count, const std::false_type&);
	static void Copy(T * des, T * src, size_t count, const std::true_type&);
	static void Copy(T * des, T * src, size_t count, const std::false_type&);

	T* m_Begin;
	T* m_End;
	T* m_MaxEnd;
	Allocator m_AllocatorInst;
};


// StackArray alias
//------------------------------------------------------------------------------
template<class T, int SIZE>
using StackArray = Array<T, StackAllocator<SIZE>>;


// ArrayIterator functions
//------------------------------------------------------------------------------
//template<typename T>
//ArrayIterator<T>::ArrayIterator(const T* element):
//m_Pointer(element)
//{
//}
//
//template<typename T>
//T& ArrayIterator<T>::operator* () const
//{
//	return *m_Pointer;
//}
//
//template<typename T>
//T* ArrayIterator<T>::operator-> () const
//{
//	return m_Pointer;
//}
//
//template<typename T>
//ArrayIterator<T>& ArrayIterator<T>::operator++ ()
//{
//	++m_Pointer;
//	return *this;
//}
//
//template<typename T>
//ArrayIterator<T>& ArrayIterator<T>::operator-- ()
//{
//	--m_Pointer;
//	return *this;
//}
//
//template<typename T>
//ArrayIterator<T>  ArrayIterator<T>::operator++ (int)
//{
//	ArrayIterator tmp(*this);
//	++m_Pointer;
//	return tmp;
//}
//
//template<typename T>
//ArrayIterator<T>  ArrayIterator<T>::operator-- (int)
//{
//	ArrayIterator tmp(*this);
//	--m_Pointer;
//	return tmp;
//}
//
//template<typename T>
//ArrayIterator<T>& ArrayIterator<T>::operator+= (int32 offset)
//{
//	m_Pointer += offset;
//	return *this;
//}
//
//template<typename T>
//ArrayIterator<T>& ArrayIterator<T>::operator-= (int32 offset)
//{
//	m_Pointer -= offset;
//	return *this;
//}
//
//template<typename T>
//ArrayIterator<T> ArrayIterator<T>::operator+ (int32 offset)
//{
//	ArrayIterator newIter(*this);
//	newIter += offset;
//	return newIter;
//}
//
//template<typename T>
//ArrayIterator<T> ArrayIterator<T>::operator- (int32 offset)
//{
//	ArrayIterator newIter(*this);
//	newIter -= offset;
//	return newIter;
//}
//
//template<typename T>
//bool ArrayIterator<T>::operator==(const ArrayIterator& other) const
//{
//	return m_Pointer == other.m_Pointer;
//}
//
//template<typename T>
//bool ArrayIterator<T>::operator!=(const ArrayIterator& other) const
//{
//	return m_Pointer != other.m_Pointer;
//}
//
//template<typename T>
//bool ArrayIterator<T>::operator<(const ArrayIterator& other) const
//{
//	return m_Pointer < other.m_Pointer;
//}
//
//template<typename T>
//bool ArrayIterator<T>::operator>(const ArrayIterator& other) const
//{
//	return m_Pointer > other.m_Pointer;
//}
//
//template<typename T>
//bool ArrayIterator<T>::operator<=(const ArrayIterator& other) const
//{
//	return m_Pointer <= other.m_Pointer;
//}
//
//template<typename T>
//bool ArrayIterator<T>::operator>=(const ArrayIterator& other) const
//{
//	return m_Pointer >= other.m_Pointer;
//}

#include "Foundation/Container/Array.inl"

//------------------------------------------------------------------------------
#endif // FOUNDATION_CONTAINER_ARRAY_H
