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
#include "Foundation/Math/Conversion.h"
#include <type_traits>


// Array
//------------------------------------------------------------------------------
template<class T, class Allocator = DefaultAllocator>
class Array
{
public:
	typedef	T *			Iter;
	typedef const T *	ConstIter;

	explicit Array();
	explicit Array(const Allocator & allocator);
	explicit Array(const T * begin, const T * end);
	explicit Array(const T * begin, const T * end, const Allocator & allocator);
	explicit Array(SIZET initSize);
	explicit Array(SIZET initSize, const Allocator & allocator);
	Array(const std::initializer_list<T> & initList);
	Array(const std::initializer_list<T> & initList, const Allocator & allocator);

	Array(const Array & other);
	template<class OtherAllocator>
	Array(const Array<T, OtherAllocator> & other);

	template<class = typename std::enable_if<std::is_move_assignable<Allocator>::value>::type>
	Array(Array && other);

	~Array();

	// Operators
	Array & operator = (const Array & other);
	template<class OtherAllocator>
	Array & operator = (const Array<T, OtherAllocator> & other);

	template<class = typename std::enable_if<std::is_move_assignable<Allocator>::value>::type>
	Array & operator = (Array && rOther);

	// Iterators
	FORCE_INLINE Iter      Begin() { return m_Begin; }
	FORCE_INLINE ConstIter Begin() const { return m_Begin; }
	FORCE_INLINE Iter      End() { return m_End; }
	FORCE_INLINE ConstIter End() const { return m_End; }
	FORCE_INLINE Iter      Last() { ASSERT(m_Begin < m_End); return m_End - 1; }
	FORCE_INLINE ConstIter Last() const { ASSERT(m_Begin < m_End); return m_End - 1; }

	FORCE_INLINE T &		operator [] (SIZET index) { ASSERT(index < GetSize()); return m_Begin[index]; }
	FORCE_INLINE const T &	operator [] (SIZET index) const { ASSERT(index < GetSize()); return m_Begin[index]; }
	FORCE_INLINE T &		TopItem() { ASSERT(m_Begin < m_End); return m_End[-1]; }
	FORCE_INLINE const T &	TopItem() const { ASSERT(m_Begin < m_End); return m_End[-1]; }
	FORCE_INLINE SIZET		Index(ConstIter iter) const { return iter - m_Begin; }
	FORCE_INLINE SIZET		Index(Iter iter) { return iter - m_Begin; }

	// C iterator
	FORCE_INLINE Iter      begin() { return m_Begin; }
	FORCE_INLINE ConstIter begin() const { return m_Begin; }
	FORCE_INLINE Iter      end() { return m_End; }
	FORCE_INLINE ConstIter end() const { return m_End; }

	// Capacity and size
	void SetCapacity(SIZET capacity);
	void SetSize(SIZET size);
	void Clear();
	void SwapItem(SIZET index1, SIZET index2);
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
	SIZET FindPos(const U & obj) const;
	SIZET FindOrAppend(const T & item);

	template<class Functor>
	T * FindIf(const Functor & cmp) const;

	// Add / Remove
	void Append(const T & item, SIZET count = 1);
	template<class U>
	void Append(const Array<U> & other);
	template<class U>
	void Append(const U * begin, const U * end);
	bool AppendUnique(const T &item);

	void Insert(SIZET index, const T & item, SIZET count = 1);
	void Insert(Iter iter, const T & item, SIZET count = 1);

	template<class... TArgs>
	void EmplaceAppend(TArgs... args);
	template<class... TArgs>
	Iter EmplaceInsert(ConstIter iter, TArgs... args);

	void Pop(SIZET count = 1);
	void PopFront(SIZET count = 1);
	void Erase(T * const iter, SIZET count = 1);
	void EraseIndex(SIZET index) { Erase(m_Begin + index); }

	// Query
	FORCE_INLINE SIZET	GetCapacity() const { return (m_MaxEnd - m_Begin); }
	FORCE_INLINE SIZET	GetSize() const { return (m_End - m_Begin); }
	FORCE_INLINE bool	IsEmpty() const { return (m_Begin == m_End); }

	// Quick operation to be implement
	void EraseSwapIndex(SIZET index, SIZET count = 1);
	void EraseSwap(T * const iter, SIZET count = 1) { EraseSwapIndex(iter - m_Begin, count); }

	// Optimization
	void Shrink();

	// Static helper
	template<class... TArgs>
	static void InPlaceConstruct(T * mem, SIZET count, TArgs... args);
	static void InPlaceDestruct(T * mem, SIZET count);
	static void UninitializedCopy(T * des, const T * src, SIZET count);
	static void UninitializedFill(T * des, const T& value, SIZET count);
	static void Move(T * des, T * src, SIZET count);
	static void Copy(T * des, T * src, SIZET count);
	static void Set(T * des, const T& value, SIZET count);

private:
	void  Swap(Array & other);
	void  Resize(SIZET size, SIZET capacity);
	void  Grow(SIZET capacity);
	SIZET GetGrowCapacity(SIZET capacity);

	T*   Allocate(SIZET size);
	T*   Reallocate(SIZET size);
	void Deallocate();

	template<class... TArgs>
	static void _InPlaceConstruct(T * mem, SIZET count, const std::false_type&, TArgs... args);
	static void _InPlaceConstruct(T * mem, SIZET count, const std::true_type&);
	static void _InPlaceDestruct(T * mem, SIZET count, const std::true_type&);
	static void _InPlaceDestruct(T * mem, SIZET count, const std::false_type&);
	static void _UninitializedCopy(T * des, const T * src, SIZET count, const std::true_type&);
	static void _UninitializedCopy(T * des, const T * src, SIZET count, const std::false_type&);
	static void _Move(T * des, T * src, SIZET count, const std::true_type&);
	static void _Move(T * des, T * src, SIZET count, const std::false_type&);
	static void _Copy(T * des, T * src, SIZET count, const std::true_type&);
	static void _Copy(T * des, T * src, SIZET count, const std::false_type&);

	T* m_Begin;
	T* m_End;
	T* m_MaxEnd;
	Allocator m_Allocator;
};


// StackArray alias
//------------------------------------------------------------------------------
template<class T, int SIZE>
using StackArray = Array<T, StackAllocator<SIZE>>;


#include "Foundation/Container/Array.inl"

//------------------------------------------------------------------------------
#endif // FOUNDATION_CONTAINER_ARRAY_H
