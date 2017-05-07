// Mem.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Mem.h"
#include "Foundation/Env/Assert.h"
#include "Foundation/Platform/Platform.h"

#include <stdlib.h>
#include <string.h>


// Alloc
//------------------------------------------------------------------------------
void * Alloc(SIZET size, SIZET alignment)
{
	ASSERT(T_IS_POWER_OF_2(alignment));
#if defined(__APPLE__)
	void * mem(nullptr);
	VERIFY(posix_memalign(&mem, alignment, size) == 0);
#else
	void * mem = _aligned_malloc(size, alignment);
#endif
	return mem;
}

// Realloc
//------------------------------------------------------------------------------
void * Realloc(void* pMem, SIZET size, SIZET alignment)
{
#if defined (__WINDOWS__)
	void * mem = _aligned_realloc(pMem, size, alignment);
#endif
	return mem;
}

// Free
//------------------------------------------------------------------------------
void Free(void * ptr)
{
#if defined(__APPLE__)
	free(ptr);
#else
	_aligned_free(ptr);
#endif
}

// Copy/Move/Set
//------------------------------------------------------------------------------
void MemCopy(void* des, const void* src, SIZET size)
{
	memcpy(des, src, size);
}

void MemMove(void* des, const void* src, SIZET size)
{
	memmove(des, src, size);
}

void MemSet(void* des, int value, SIZET size)
{
	memset(des, value, size);
}

//------------------------------------------------------------------------------
