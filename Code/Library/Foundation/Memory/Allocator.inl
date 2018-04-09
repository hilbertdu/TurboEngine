// Allocator.inl
//------------------------------------------------------------------------------


// Allocator
//------------------------------------------------------------------------------
template<class AllocatorS>
template<class T>
T * Allocator<AllocatorS>::AllocateT(SIZET size, SIZET alignment)
{
#if (T_MEM_TRACKER)
	return MemStamp(__FILE__, __LINE__) * (T *)AllocatorS::Allocate(size * sizeof(T));
#else
	return (T *)AllocatorS::Allocate(size * sizeof(T));
#endif
}

template<class AllocatorS>
template<class T>
T * Allocator<AllocatorS>::ReallocateT(T * ptr, SIZET size, SIZET alignment)
{
#if (T_MEM_TRACKER)
	return MemStamp(__FILE__, __LINE__) * (T *)AllocatorS::Reallocate(T * ptr, size * sizeof(T));
#else
	return (T *)AllocatorS::Reallocate(T * ptr, size * sizeof(T));
#endif
}

template<class AllocatorS>
template<class T>
void Allocator<AllocatorS>::FreeT(T * ptr)
{
	AllocatorS::Free(ptr);
}


// StackAllocator
//------------------------------------------------------------------------------
template<uint32 RESERVED, bool SUPPORT_OVERFLOW>
StackSizeAllocator<RESERVED, SUPPORT_OVERFLOW>::StackSizeAllocator()
	: m_StackFreeIdx(0)
	, m_StackFreeLastIdx(0)
{
}

template<uint32 RESERVED, bool SUPPORT_OVERFLOW>
void* StackSizeAllocator<RESERVED, SUPPORT_OVERFLOW>::Allocate(SIZET size, SIZET alignment)
{
	ASSERT(size > 0);
	m_StackFreeIdx = GetAlignFreeIdx(alignment);
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
void* StackSizeAllocator<RESERVED, SUPPORT_OVERFLOW>::Reallocate(void* pMem, SIZET size, SIZET alignment)
{
	ASSERT(pMem);
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
		m_StackFreeIdx = GetAlignFreeIdx(alignment);
		if (m_StackFreeIdx + size <= RESERVED)
		{
			MemMove((void *)m_StackMem[m_StackFreeIdx], pMem, size);
			m_StackFreeLastIdx = m_StackFreeIdx;
			m_StackFreeIdx += (uint32)size;
			return (void *)&m_StackMem[m_StackFreeIdx - size];
		}
	}

	// Reallocate from heap
	m_StackFreeIdx = RESERVED;
	ASSERT(SUPPORT_OVERFLOW);
	return REALLOC(pMem, size);
}

template<uint32 RESERVED, bool SUPPORT_OVERFLOW>
void StackSizeAllocator<RESERVED, SUPPORT_OVERFLOW>::Free(void* pMem)
{
	ASSERT(pMem);
	if (!IsInStack(pMem))
	{
		FREE(pMem);
	}
}

template<uint32 RESERVED, bool SUPPORT_OVERFLOW>
bool StackSizeAllocator<RESERVED, SUPPORT_OVERFLOW>::IsInStack(void* pMem) const
{
	return (UINTPTR)pMem >= (UINTPTR)&m_StackMem && (UINTPTR)pMem <= (UINTPTR)&m_StackMem[RESERVED - 1];
}

template<uint32 RESERVED, bool SUPPORT_OVERFLOW>
uint32 StackSizeAllocator<RESERVED, SUPPORT_OVERFLOW>::GetAlignFreeIdx(SIZET alignment)
{
	if (m_StackFreeIdx < RESERVED)
	{
		UINTPTR alignedAddr = T_MEM_ALIGN_ARB(&m_StackMem[m_StackFreeIdx], alignment);
		m_StackFreeIdx = (uint32)(alignedAddr - (UINTPTR)m_StackMem);
	}
	return m_StackFreeIdx;
}


// PoolAllocator
//------------------------------------------------------------------------------
template<class AFORM, class CATEGORY>
/*explicit*/ PoolSizeAllocator<AFORM, CATEGORY>::PoolSizeAllocator()
	: m_PoolBlock(TNEW(MemPoolBlock(AFORM::AllocSize, AFORM::Alignment)))
{
#if T_MEM_STATISTICS
	m_PoolBlock->SetCategoryName(CATEGORY::Str);
#endif
}

template<class AFORM, class CATEGORY>
void* PoolSizeAllocator<AFORM, CATEGORY>::Allocate(SIZET size, SIZET alignment)
{
	return m_PoolBlock->Alloc(size);
}

template<class AFORM, class CATEGORY>
void* PoolSizeAllocator<AFORM, CATEGORY>::Reallocate(void* pMem, SIZET size, SIZET alignment)
{
	return m_PoolBlock->Alloc(size);
}

template<class AFORM, class CATEGORY>
void PoolSizeAllocator<AFORM, CATEGORY>::Free(void* pMem)
{
	m_PoolBlock->Free(pMem);
}

template<class AFORM, class CATEGORY>
PoolDeletor<AFORM, CATEGORY> PoolSizeAllocator<AFORM, CATEGORY>::GetDeletor()
{
	return PoolDeletor<AFORM, CATEGORY>(this);
}
//------------------------------------------------------------------------------

