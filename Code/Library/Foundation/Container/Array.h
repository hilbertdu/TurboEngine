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
	explicit Array(SIZET initSize, const T & element);
	Array(std::initializer_list<T> initList);
	Array(std::initializer_list<T> initList, const Allocator & allocator);
	Array(const Array & other);
	template<class OtherAllocator>
	Array(const Array<T, OtherAllocator> & other);
	Array(Array && other);

	~Array();

	// Operators
	Array & operator = (const Array & other);
	Array & operator = (Array && rOther);
	template<class OtherAllocator>
	Array & operator = (const Array<T, OtherAllocator> & other);

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
	FORCE_INLINE Iter		Iterator(SIZET index) const { return m_Begin + index; }

	// C iterator
	FORCE_INLINE Iter      begin() { return m_Begin; }
	FORCE_INLINE ConstIter begin() const { return m_Begin; }
	FORCE_INLINE Iter      end() { return m_End; }
	FORCE_INLINE ConstIter end() const { return m_End; }	

	// Capacity and size
	void SetCapacity(SIZET capacity);
	void SetSize(SIZET size);
	void Clear();
	void SwapItem(Iter iter1, Iter iter2);
	void Swap(Array & other);

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
	void Append(T && item);
	template<class U>
	void Append(const Array<U> & other);
	template<class U>
	void Append(const U * begin, const U * end);
	bool AppendUnique(const T & item);

	Iter Insert(Iter iter, const T & item, SIZET count = 1);
	Iter Insert(Iter iter, T && item);

	template<class... TArgs>
	Iter EmplaceAppend(TArgs&&... args);
	template<class... TArgs>
	Iter EmplaceInsert(Iter iter, TArgs&&... args);

	void Pop(SIZET count = 1);
	void PopFront(SIZET count = 1);
	void Erase(T * const iter, SIZET count = 1);
	void EraseIndex(SIZET index) { Erase(m_Begin + index); }

	// Query
	FORCE_INLINE SIZET	GetCapacity() const { return (m_MaxEnd - m_Begin); }
	FORCE_INLINE SIZET	GetSize() const { return (m_End - m_Begin); }
	FORCE_INLINE bool	IsEmpty() const { return (m_Begin == m_End); }

	// Quick operation to be implement
	FORCE_INLINE void EraseSwap(T * const iter, SIZET count = 1) { EraseSwapIndex(iter - m_Begin, count); }
	void EraseSwapIndex(SIZET index, SIZET count = 1);

	// Optimization
	void Shrink();

	// Get raw
	FORCE_INLINE T * GetPointer() { return m_Begin; }

	// Static helper
	template<class... TArgs>
	FORCE_INLINE static void InPlaceConstruct(T * mem, SIZET count, TArgs&&... args);
	FORCE_INLINE static void InPlaceConstruct(T * mem, SIZET count);
	FORCE_INLINE static void InPlaceDestruct(T * mem, SIZET count);
	FORCE_INLINE static void UninitializedCopy(T * des, const T * src, SIZET count);
	FORCE_INLINE static void UninitializedMove(T * des, T * src, SIZET count);
	FORCE_INLINE static void UninitializedFill(T * des, const T& value, SIZET count);
	FORCE_INLINE static void Move(T * des, T * src, SIZET count);
	FORCE_INLINE static void Copy(T * des, T * src, SIZET count);
	FORCE_INLINE static void Set(T * des, const T& value, SIZET count);

private:
	void  Resize(SIZET size, SIZET capacity);
	FORCE_INLINE SIZET GetGrowCapacity(SIZET capacity);
	FORCE_INLINE void  Grow(SIZET capacity);

	FORCE_INLINE T*   Allocate(SIZET size);
	FORCE_INLINE T*   Reallocate(SIZET size);
	FORCE_INLINE void Deallocate();

	static void _InPlaceConstruct(T * mem, SIZET count, const std::true_type&);
	template<class... TArgs>
	static void _InPlaceConstruct(T * mem, SIZET count, const std::false_type&, TArgs&&... args);
	static void _InPlaceDestruct(T * mem, SIZET count, const std::true_type&);
	static void _InPlaceDestruct(T * mem, SIZET count, const std::false_type&);
	static void _UninitializedCopy(T * des, const T * src, SIZET count, const std::true_type&);
	static void _UninitializedCopy(T * des, const T * src, SIZET count, const std::false_type&);
	static void _UninitializedMove(T * des, T * src, SIZET count, const std::true_type&);
	static void _UninitializedMove(T * des, T * src, SIZET count, const std::false_type&);
	static void _Move(T * des, T * src, SIZET count, const std::true_type&);
	static void _Move(T * des, T * src, SIZET count, const std::false_type&);
	static void _Copy(T * des, T * src, SIZET count, const std::true_type&);
	static void _Copy(T * des, const T * src, SIZET count, const std::false_type&);

	Allocator m_Allocator;
	T* m_Begin;
	T* m_End;
	T* m_MaxEnd;
};


// StackArray alias
//------------------------------------------------------------------------------
template<class T, int SIZE>
using StackArray = Array<T, StackAllocator<SIZE>>;


#include "Foundation/Container/Array.inl"

//------------------------------------------------------------------------------
#endif // FOUNDATION_CONTAINER_ARRAY_H
