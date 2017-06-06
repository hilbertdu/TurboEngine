// Allocator.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_MEMORY_ALLOCATOR_H
#define FOUNDATION_MEMORY_ALLOCATOR_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/Env/Assert.h"
#include "Foundation/Memory/Mem.h"
#include "Foundation/Memory/MemPool.h"
#include "Foundation/Memory/MemPoolBlock.h"
#include "Foundation/Template/String.h"
#include "Foundation/Container/SmartPtr.h"
#include "Foundation/Memory/Deletor.h"
#include "Foundation/Logging/Logger.h"
#include <type_traits>

#define DefaultAllocator HeapAllocator


// AlignAlloc
//------------------------------------------------------------------------------
struct AllocForm
{
public:
	AllocForm(SIZET size, SIZET alignment = T_MEM_DEFAULT_ALIGN)
		: m_AllocSize(size)
		, m_Alignment(alignment)
	{}

	SIZET GetAlignBlock() const
	{
		return T_MEM_ALIGN_ARB(m_AllocSize, m_Alignment);
	}

	bool operator==(const AllocForm & rOther) const
	{
		return GetAlignBlock() == rOther.GetAlignBlock();
	}

	SIZET m_AllocSize;
	SIZET m_Alignment;
};


template<SIZET ALLOC_SIZE, SIZET ALIGNMENT = T_MEM_DEFAULT_ALIGN>
struct TAllocForm
{
	static const SIZET AllocSize{ ALLOC_SIZE };
	static const SIZET Alignment{ ALIGNMENT };
};


template<class AllocatorS>
class Allocator : public AllocatorS
{
public:
	template<class T>
	T * AllocateT(SIZET size, SIZET alignment = T_MEM_DEFAULT_ALIGN);
	template<class T>
	T * ReallocateT(T * ptr, SIZET size, SIZET alignment = T_MEM_DEFAULT_ALIGN);
	template<class T>
	void FreeT(T * ptr);
};


class HeapSizeAllocator
{
public:
	void* Allocate(SIZET size, SIZET alignment = T_MEM_DEFAULT_ALIGN) { return ALLOC(size, alignment); }
	void* Reallocate(void* pMem, SIZET size, SIZET alignment = T_MEM_DEFAULT_ALIGN) { return REALLOC(pMem, size); }
	void  Free(void* pMem) { FREE(pMem); }
};


template<uint32 RESERVED = 256, bool SUPPORT_OVERFLOW = true>
class StackSizeAllocator
{
public:
	explicit StackSizeAllocator();
	StackSizeAllocator(const StackSizeAllocator&) = delete;
	StackSizeAllocator(StackSizeAllocator&&) = delete;

	void* Allocate(SIZET size, SIZET alignment = T_MEM_DEFAULT_ALIGN);
	void* Reallocate(void* pMem, SIZET size, SIZET alignment = T_MEM_DEFAULT_ALIGN);
	void  Free(void* pMem);
	bool  IsInStack(void* pMem) const;

private:
	char	m_StackMem[RESERVED];
	uint32	m_StackFreeIdx;
	uint32	m_StackFreeLastIdx;
};


template<class AFORM, class CATEGORY = CONSTSTR("Default")>
class PoolSizeAllocator
{
public:
	explicit PoolSizeAllocator();

	void* Allocate(SIZET size, SIZET alignment = T_MEM_DEFAULT_ALIGN);
	void* Reallocate(void* pMem, SIZET size, SIZET alignment = T_MEM_DEFAULT_ALIGN);
	void  Free(void* pMem);

	PoolDeletor<AFORM, CATEGORY> GetDeletor();

private:
	StrongPtr<MemPoolBlock> m_PoolBlock;
};


using HeapAllocator = Allocator<HeapSizeAllocator>;

template<uint32 RESERVED = 256, bool SUPPORT_OVERFLOW = true>
using StackAllocator = Allocator<StackSizeAllocator<RESERVED, SUPPORT_OVERFLOW>>;

template<class AFORM, class CATEGORY = CONSTSTR("Default")>
using PoolAllocator = Allocator<PoolSizeAllocator<AFORM, CATEGORY>>;


#include "Foundation/Memory/Allocator.inl"

//------------------------------------------------------------------------------
#endif // FOUNDATION_MEMORY_ALLOCATOR_H