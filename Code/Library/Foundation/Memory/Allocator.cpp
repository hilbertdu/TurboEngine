// Allocator
//------------------------------------------------------------------------------
// Includes
//------------------------------------------------------------------------------
#include "Mem.h"
#include "Allocator.h"

// HeapAllocator
//------------------------------------------------------------------------------
/*virtual*/ void* HeapAllocator::Allocate(size_t size)
{
	return ALLOC(size);
}

/*virtual*/ void* HeapAllocator::AllocateAligned(size_t size, size_t alignment)
{
	return ALLOC(size, alignment);
}

/*virtual*/ void* HeapAllocator::Reallocate(void* pMem, size_t size)
{
	return REALLOC(pMem, size);
}

/*virtual*/ void* HeapAllocator::ReallocateAligned(void* pMem, size_t size, size_t alignment)
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

