// Allocator
//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "Mem.h"
#include "Allocator.h"

// HeapAllocator
//------------------------------------------------------------------------------
/*virtual*/ void* HeapAllocator::Allocate(SIZET size)
{
	return ALLOC(size);
}

/*virtual*/ void* HeapAllocator::AllocateAligned(SIZET size, SIZET alignment)
{
	return ALLOC(size, alignment);
}

/*virtual*/ void* HeapAllocator::Reallocate(void* pMem, SIZET size)
{
	return REALLOC(pMem, size);
}

/*virtual*/ void* HeapAllocator::ReallocateAligned(void* pMem, SIZET size, SIZET alignment)
{
	return REALLOC(pMem, size, alignment);
}

/*virtual*/ void HeapAllocator::Free(void* pMem)
{
	FREE(pMem);
}

/*virtual*/ void HeapAllocator::FreeAligned(void* pMem)
{
	FREE(pMem);
}
//------------------------------------------------------------------------------

