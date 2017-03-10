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
#include <type_traits>

#define DefaultAllocator HeapAllocator


// AlignAlloc
//------------------------------------------------------------------------------
struct AllocForm
{
public:
	AllocForm(SIZET size, SIZET alignment = sizeof(void *))
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


template<SIZET ALLOC_SIZE, SIZET ALIGNMENT = sizeof(void *)>
struct TAllocForm
{
	static const SIZET AllocSize{ ALLOC_SIZE };
	static const SIZET Alignment{ ALIGNMENT };
};


class HeapAllocator
{
public:
	virtual void* Allocate(SIZET size);
	virtual void* AllocateAligned(SIZET size, SIZET alignment);

	virtual void* Reallocate(void* pMem, SIZET size);
	virtual void* ReallocateAligned(void* pMem, SIZET size, SIZET alignment);

	virtual void Free(void* pMem);
	virtual void FreeAligned(void* pMem);
};


template<uint32 RESERVED = 256, bool SUPPORT_OVERFLOW = true>
class StackAllocator
{
public:
	explicit StackAllocator();

	virtual void* Allocate(SIZET size);
	virtual void* AllocateAligned(SIZET size, SIZET alignment);

	virtual void* Reallocate(void* pMem, SIZET size);
	virtual void* ReallocateAligned(void* pMem, SIZET size, SIZET alignment);

	virtual void Free(void* pMem);
	virtual void FreeAligned(void* pMem);

	bool IsInStack(void* pMem) const;

private:
	char	m_StackMem[RESERVED];
	uint32	m_StackFreeIdx;
	uint32	m_StackFreeLastIdx;
};


template<class AFORM, class CATEGORY = CONSTSTR("Default")>
class PoolAllocator
{
public:
	explicit PoolAllocator();

	virtual void* Allocate(SIZET size);
	virtual void* AllocateAligned(SIZET size, SIZET alignment);

	virtual void* Reallocate(void* pMem, SIZET size);
	virtual void* ReallocateAligned(void* pMem, SIZET size, SIZET alignment);

	virtual void Free(void* pMem);
	virtual void FreeAligned(void* pMem);

	PoolDeletor<AFORM, CATEGORY> GetDeletor();

private:
	StrongPtr<MemPoolBlock> m_PoolBlock;
};


// StackAllocator
//------------------------------------------------------------------------------
template<uint32 RESERVED, bool SUPPORT_OVERFLOW>
StackAllocator<RESERVED, SUPPORT_OVERFLOW>::StackAllocator()
	: m_StackFreeIdx(0)
	, m_StackFreeLastIdx(0)
{
}

template<uint32 RESERVED, bool SUPPORT_OVERFLOW>
/*virtual*/ void* StackAllocator<RESERVED, SUPPORT_OVERFLOW>::Allocate(SIZET size)
{
	ASSERT(size > 0);

	if (m_StackFreeIdx + size <= RESERVED)
	{
		m_StackFreeLastIdx = m_StackFreeIdx;
		m_StackFreeIdx += (uint32)size;
		return (void *)&m_StackMem[m_StackFreeIdx - size];
	}

	// Allocate from heap
	ASSERT(SUPPORT_OVERFLOW);
	return ALLOC(size);
}

template<uint32 RESERVED, bool SUPPORT_OVERFLOW>
/*virtual*/ void* StackAllocator<RESERVED, SUPPORT_OVERFLOW>::AllocateAligned(SIZET size, SIZET alignment)
{
	UINTPTR alignedAddr = T_MEM_ALIGN_ARB(&m_StackMem[m_StackFreeIdx], alignment);
	m_StackFreeIdx = (uint32)(alignedAddr - (UINTPTR)m_StackMem);

	ASSERT(size > 0);
	if (m_StackFreeIdx + size <= RESERVED)
	{
		m_StackFreeLastIdx = m_StackFreeIdx;
		m_StackFreeIdx += (uint32)size;
		return (void *)&m_StackMem[m_StackFreeIdx - size];
	}

	// Allocate from heap
	ASSERT(SUPPORT_OVERFLOW);
	return ALLOC(size, alignment);
}

template<uint32 RESERVED, bool SUPPORT_OVERFLOW>
/*virtual*/ void* StackAllocator<RESERVED, SUPPORT_OVERFLOW>::Reallocate(void* pMem, SIZET size)
{
	if (pMem == m_StackMem + m_StackFreeLastIdx)
	{
		// Try to increase in place
		if (m_StackFreeLastIdx + size < RESERVED)
		{
			m_StackFreeIdx = (uint32)(m_StackFreeLastIdx + size);
			return pMem;
		}
	}
	else
	{
		// Copy to upper
		if (m_StackFreeIdx + size <= RESERVED)
		{
			MemMove((void *)m_StackMem[m_StackFreeIdx], pMem, size);

			m_StackFreeLastIdx = m_StackFreeIdx;
			m_StackFreeIdx += (uint32)size;
			return (void *)&m_StackMem[m_StackFreeIdx - size];
		}
	}

	// Reallocate from heap
	ASSERT(SUPPORT_OVERFLOW);
	return REALLOC(pMem, size);
}

template<uint32 RESERVED, bool SUPPORT_OVERFLOW>
/*virtual*/ void* StackAllocator<RESERVED, SUPPORT_OVERFLOW>::ReallocateAligned(void* pMem, SIZET size, SIZET alignment)
{
	return Reallocate(pMem, size);
}

template<uint32 RESERVED, bool SUPPORT_OVERFLOW>
/*virtual*/ void StackAllocator<RESERVED, SUPPORT_OVERFLOW>::Free(void* pMem)
{
	ASSERT(pMem);
	if ((UINTPTR)pMem < (UINTPTR)&m_StackMem || (UINTPTR)pMem > (UINTPTR)&m_StackMem[RESERVED - 1])
	{
		FREE(pMem);
	}
}

template<uint32 RESERVED, bool SUPPORT_OVERFLOW>
/*virtual*/ void StackAllocator<RESERVED, SUPPORT_OVERFLOW>::FreeAligned(void* pMem)
{
	Free(pMem);
}

template<uint32 RESERVED, bool SUPPORT_OVERFLOW>
bool StackAllocator<RESERVED, SUPPORT_OVERFLOW>::IsInStack(void* pMem) const
{
	return (UINTPTR)pMem >= (UINTPTR)&m_StackMem && (UINTPTR)pMem <= (UINTPTR)&m_StackMem[RESERVED - 1];
}


// PoolAllocator
//------------------------------------------------------------------------------
template<class AFORM, class CATEGORY>
/*explicit*/ PoolAllocator<AFORM, CATEGORY>::PoolAllocator()
	: m_PoolBlock(TNEW(MemPoolBlock(AFORM::AllocSize, AFORM::Alignment)))
{
#if T_MEM_STATISTICS
	m_PoolBlock->SetCategoryName(CATEGORY::Str);
#endif
}

template<class AFORM, class CATEGORY>
/*virtual*/ void* PoolAllocator<AFORM, CATEGORY>::Allocate(SIZET size)
{
	return m_PoolBlock->Alloc(size);
}

template<class AFORM, class CATEGORY>
/*virtual*/ void* PoolAllocator<AFORM, CATEGORY>::AllocateAligned(SIZET size, SIZET alignment)
{
	return m_PoolBlock->Alloc(size);
}

template<class AFORM, class CATEGORY>
/*virtual*/ void* PoolAllocator<AFORM, CATEGORY>::Reallocate(void* pMem, SIZET size)
{
	return m_PoolBlock->Alloc(size);
}

template<class AFORM, class CATEGORY>
/*virtual*/ void* PoolAllocator<AFORM, CATEGORY>::ReallocateAligned(void* pMem, SIZET size, SIZET alignment)
{
	return m_PoolBlock->Alloc(size);
}

template<class AFORM, class CATEGORY>
/*virtual*/ void PoolAllocator<AFORM, CATEGORY>::Free(void* pMem)
{
	m_PoolBlock->Free(pMem);
}

template<class AFORM, class CATEGORY>
/*virtual*/ void PoolAllocator<AFORM, CATEGORY>::FreeAligned(void* pMem)
{
	m_PoolBlock->Free(pMem);
}

template<class AFORM, class CATEGORY>
PoolDeletor<AFORM, CATEGORY> PoolAllocator<AFORM, CATEGORY>::GetDeletor()
{
	return PoolDeletor<AFORM, CATEGORY>(this);
}

//------------------------------------------------------------------------------
#endif // FOUNDATION_MEMORY_ALLOCATOR_H