// Array.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_CONTAINER_BITARRAY_H
#define FOUNDATION_CONTAINER_BITARRAY_H

// Include
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/Platform/Misc.h"
#include "Foundation/Memory/Allocator.h"
#include "Foundation/Memory/Mem.h"
#include "Foundation/Container/Array.h"
#include <type_traits>

using InnerType = uint8;

// BitArrayElement
//------------------------------------------------------------------------------
template<bool Const = true>
class BitArrayElement
{
public:
	BitArrayElement(InnerType& element, InnerType mask);

	// Access
	inline operator bool() const;

	// Compare
	inline bool operator==(bool bValue) const;
	inline bool operator!=(bool bValue) const;
	template<bool rConst>
	inline bool operator==(const BitArrayElement<rConst>& rOther) const;
	template<bool rConst>
	inline bool operator!=(const BitArrayElement<rConst>& rOther) const;

private:
	InnerType& m_Element;
	InnerType  m_Mask;
};


// BitArrayIterator
//------------------------------------------------------------------------------
template<bool Const = true>
class BitArrayIterator
{
public:
	typedef BitArrayElement<true>	ConstElement;
	typedef BitArrayElement<false>	Element;

	static constexpr SIZET EBITSIZE = sizeof(InnerType) * 8;
	static_assert(T_IS_POWER_OF_2(EBITSIZE) && EBITSIZE <= sizeof(int) * 8, "bit size is large than int size");

	explicit BitArrayIterator() = default;
	explicit BitArrayIterator(InnerType* element, InnerType mask);
	BitArrayIterator(const BitArrayIterator& rOther) = default;
	BitArrayIterator(const BitArrayIterator<!Const>& rOther);

	BitArrayIterator& operator=(const BitArrayIterator& rOther) = default;
	BitArrayIterator& operator=(const BitArrayIterator<!Const>& rOther);

	ConstElement operator*() const;

	template<class = std::enable_if<!Const>::type>
	Element operator*();

	BitArrayIterator& operator++();
	BitArrayIterator  operator++(int);
	BitArrayIterator& operator--();
	BitArrayIterator  operator--(int);

	bool operator==(const BitArrayIterator& rOther) const;
	bool operator!=(const BitArrayIterator& rOther) const;
	bool operator<(const BitArrayIterator& rOther) const;
	bool operator>(const BitArrayIterator& rOther) const;
	bool operator<=(const BitArrayIterator& rOther) const;
	bool operator>=(const BitArrayIterator& rOther) const;

protected:
	InnerType* m_Element;
	InnerType  m_Mask;	
};


// BitArray
//------------------------------------------------------------------------------
template<class Allocator = DefaultAllocator>
class BitArray
{
public:
	typedef BitArrayElement<false>	Element;
	typedef BitArrayElement<true>	ConstElement;
	typedef BitArrayIterator<false> Iter;
	typedef BitArrayIterator<true>	ConstIter;
	typedef Array<InnerType, Allocator>	InnerArray;

	static constexpr SIZET EBITSIZE = sizeof(InnerType) * 8;
	static_assert(T_IS_POWER_OF_2(EBITSIZE) && EBITSIZE <= sizeof(int) * 8, "bit size is large than int size");

	// Constructors
	BitArray();
	explicit BitArray(const Allocator& allocator);
	explicit BitArray(SIZET initSize);		
	template<class OtherAllocator>
	BitArray(const BitArray<OtherAllocator>& other);

	BitArray(const BitArray& other) = default;
	
	// Operators
	BitArray& operator=(const BitArray& other) = default;
	template<class OtherAllocator>
	BitArray& operator=(const BitArray<OtherAllocator>& other);

	// Set
	void Set(SIZET index, bool value);
	void Toggle(SIZET index);	

	void SetAll(bool value);
	void ToggleAll();

	void SetRange(SIZET begin, SIZET end, bool value);
	void ToggleRange(SIZET beign, SIZET end);

	void Append(bool value, SIZET count = 1);

	void SetSize(SIZET size);
	void Shrink();

	// Iterator
	FORCE_INLINE Iter		Begin();
	FORCE_INLINE Iter		End();
	FORCE_INLINE ConstIter	Begin() const;
	FORCE_INLINE ConstIter	End() const;

	// Access
	FORCE_INLINE Element		operator[](SIZET index);
	FORCE_INLINE ConstElement	operator[](SIZET index) const;

	FORCE_INLINE bool  IsEmpty() const { return m_Content.IsEmpty(); }
	FORCE_INLINE SIZET GetSize() const { return m_Size; }
	FORCE_INLINE SIZET GetCapacity() const { return m_Content.GetCapacity() * EBITSIZE; }

private:
	FORCE_INLINE SIZET GetInnerIndex(SIZET index) const { return index >> Math::Log2(EBITSIZE); }	// Get array index
	FORCE_INLINE InnerType GetInnerOffset(SIZET index) const { return (InnerType)(index & (EBITSIZE - 1)); }	// Get bitset offset

	void SetInnerRange(SIZET index, SIZET begin, SIZET end, bool value);
	void ToggleInnerRange(SIZET index, SIZET begin, SIZET end);

	Array<InnerType, Allocator> m_Content;
	SIZET m_Size;
};


#include "Foundation/Container/BitArray.inl"

//------------------------------------------------------------------------------
#endif // FOUNDATION_CONTAINER_BITARRAY_H