// Allocator.h
//------------------------------------------------------------------------------
#pragma once

#include "Foundation/Platform/Types.h"
#include "Foundation/Env/Assert.h"
#include "Foundation/Memory/Mem.h"
#include "Foundation/Memory/MemPool.h"
#include "Foundation/Memory/MemPoolBlock.h"
#include <type_traits>

#define DefaultAllocator HeapAllocator


class IAllocator
{
public:
	enum {
		SUPPORT_GROWTH = false
	};

	virtual void* Allocate(size_t size) = 0;
	virtual void* AllocateAligned(size_t size, size_t alignment) = 0;

	virtual void* Reallocate(void* pMem, size_t size) = 0;
	virtual void* ReallocateAligned(void* pMem, size_t size, size_t alignment) = 0;

	virtual void Free(void* pMem) = 0;
	virtual void FreeAligned(void* pMem) = 0;
};


class HeapAllocator: public IAllocator
{
public:
	enum {
		SUPPORT_GROWTH = true
	};

	virtual void* Allocate(size_t size);
	virtual void* AllocateAligned(size_t size, size_t alignment);

	virtual void* Reallocate(void* pMem, size_t size);
	virtual void* ReallocateAligned(void* pMem, size_t size, size_t alignment);

	virtual void Free(void* pMem);
	virtual void FreeAligned(void* pMem);
};


template<int RESERVED = 256, bool SUPPORT_OVERFLOW = true>
class StackAllocator: public IAllocator
{
public:
	enum {
		SUPPORT_GROWTH = SUPPORT_OVERFLOW
	};

	explicit StackAllocator();

	StackAllocator(const StackAllocator& stackAllocator) = delete;

	virtual void* Allocate(size_t size);
	virtual void* AllocateAligned(size_t size, size_t alignment);

	virtual void* Reallocate(void* pMem, size_t size);
	virtual void* ReallocateAligned(void* pMem, size_t size, size_t alignment);

	virtual void Free(void* pMem);
	virtual void FreeAligned(void* pMem);
private:
	char	m_StackMem[RESERVED];
	UINTPTR	m_StackFree;
	UINTPTR	m_StackFreeLast;
};


template<int BLOCK_SIZE, int ALIGNMENT = sizeof(void *)>
class PoolAllocator : public IAllocator
{
public:
	enum {
		SUPPORT_GROWTH = true
	};

	explicit PoolAllocator();

	virtual void* Allocate(size_t size);
	virtual void* AllocateAligned(size_t size, size_t alignment);

	virtual void* Reallocate(void* pMem, size_t size);
	virtual void* ReallocateAligned(void* pMem, size_t size, size_t alignment);

	virtual void Free(void* pMem);
	virtual void FreeAligned(void* pMem);

	inline MemPoolBlock* GetPoolBlock() { return m_PoolBlock; };

private:
	MemPoolBlock* m_PoolBlock;
};



// StackAllocator
//------------------------------------------------------------------------------
template<int RESERVED, bool SUPPORT_OVERFLOW>
StackAllocator<RESERVED, SUPPORT_OVERFLOW>::StackAllocator()
	: m_StackFree((UINTPTR)m_StackMem)
	, m_StackFreeLast(0)
{
}

template<int RESERVED, bool SUPPORT_OVERFLOW>
/*virtual*/ void* StackAllocator<RESERVED, SUPPORT_OVERFLOW>::Allocate(size_t size)
{
	ASSERT(size > 0);

	if (!SUPPORT_OVERFLOW || m_StackFree + size <= (UINTPTR)m_StackMem + RESERVED)
	{
		m_StackFreeLast = m_StackFree;
		m_StackFree += size;
		return (void *)m_StackFreeLast;
	}

	// Allocate from heap
	ASSERT(SUPPORT_OVERFLOW);
	return ALLOC(size);
}

template<int RESERVED, bool SUPPORT_OVERFLOW>
/*virtual*/ void* StackAllocator<RESERVED, SUPPORT_OVERFLOW>::AllocateAligned(size_t size, size_t alignment)
{
	m_StackFreeLast = T_MEM_ALIGN_ARB(m_StackFree, alignment);

	ASSERT(size > 0);
	if (!SUPPORT_OVERFLOW || m_StackFree + size <= (UINTPTR)m_StackMem + RESERVED)
	{
		m_StackFree += size;
		return (void *)m_StackFreeLast;
	}

	// Allocate from heap
	ASSERT(SUPPORT_OVERFLOW);
	return ALLOC(size, alignment);
}

template<int RESERVED, bool SUPPORT_OVERFLOW>
/*virtual*/ void* StackAllocator<RESERVED, SUPPORT_OVERFLOW>::Reallocate(void* pMem, size_t size)
{
	if ((UINTPTR)pMem == m_StackFreeLast)
	{
		// Try to increase in place
		if ((UINTPTR)pMem + size <= (UINTPTR)m_StackMem + RESERVED)
		{
			return pMem;
		}
	}
	else
	{
		// Copy to upper
		if (m_StackFree + size <= (UINTPTR)m_StackMem + RESERVED)
		{
			memmove((void *)m_StackFree, pMem, size);

			m_StackFree += size;
			return (void *)m_StackFreeLast;
		}
	}

	// Reallocate from heap
	ASSERT(SUPPORT_OVERFLOW);
	return REALLOC(pMem, size);
}

template<int RESERVED, bool SUPPORT_OVERFLOW>
/*virtual*/ void* StackAllocator<RESERVED, SUPPORT_OVERFLOW>::ReallocateAligned(void* pMem, size_t size, size_t alignment)
{
	return Reallocate(pMem, size);
}

template<int RESERVED, bool SUPPORT_OVERFLOW>
/*virtual*/ void StackAllocator<RESERVED, SUPPORT_OVERFLOW>::Free(void* pMem)
{
	ASSERT(pMem);
	if (pMem < m_StackMem || (UINTPTR)pMem >= (UINTPTR)m_StackMem + RESERVED)
	{
		FREE(pMem);
	}
}

template<int RESERVED, bool SUPPORT_OVERFLOW>
/*virtual*/ void StackAllocator<RESERVED, SUPPORT_OVERFLOW>::FreeAligned(void* pMem)
{
	Free(pMem);
}


// PoolAllocator
//------------------------------------------------------------------------------
template<int BLOCK_SIZE, int ALIGNMENT>
PoolAllocator<BLOCK_SIZE, ALIGNMENT>::PoolAllocator()
	: m_PoolBlock(MemPool::GetPoolBlock(T_MEM_ALIGN_ARB(BLOCK_SIZE, ALIGNMENT)))
{
}

template<int BLOCK_SIZE, int ALIGNMENT>
/*virtual*/ void* PoolAllocator<BLOCK_SIZE, ALIGNMENT>::Allocate(size_t size)
{
	return m_PoolBlock->Alloc(size);
}

template<int BLOCK_SIZE, int ALIGNMENT>
/*virtual*/ void* PoolAllocator<BLOCK_SIZE, ALIGNMENT>::AllocateAligned(size_t size, size_t alignment)
{
	return m_PoolBlock->Alloc(size);
}

template<int BLOCK_SIZE, int ALIGNMENT>
/*virtual*/ void* PoolAllocator<BLOCK_SIZE, ALIGNMENT>::Reallocate(void* pMem, size_t size)
{
	return m_PoolBlock->Alloc(size);
}

template<int BLOCK_SIZE, int ALIGNMENT>
/*virtual*/ void* PoolAllocator<BLOCK_SIZE, ALIGNMENT>::ReallocateAligned(void* pMem, size_t size, size_t alignment)
{
	return m_PoolBlock->Alloc(size);
}

template<int BLOCK_SIZE, int ALIGNMENT>
/*virtual*/ void PoolAllocator<BLOCK_SIZE, ALIGNMENT>::Free(void* pMem)
{
	m_PoolBlock->Free(pMem);
}

template<int BLOCK_SIZE, int ALIGNMENT>
/*virtual*/ void PoolAllocator<BLOCK_SIZE, ALIGNMENT>::FreeAligned(void* pMem)
{
	m_PoolBlock->Free(pMem);
}


//------------------------------------------------------------------------------