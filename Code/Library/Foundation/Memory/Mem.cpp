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
void * Alloc(SIZET size)
{
	return AllocFileLine(size, sizeof(void *), "Unknown", 0);
}

// Alloc
//------------------------------------------------------------------------------
void * Alloc(SIZET size, SIZET alignment)
{
	return AllocFileLine(size, alignment, "Unknown", 0);
}

// AllocFileLine
//------------------------------------------------------------------------------
void * AllocFileLine(SIZET size, const char * file, int line)
{
	return AllocFileLine(size, sizeof(void *), file, line);
}

// AllocFileLine
//------------------------------------------------------------------------------
void * AllocFileLine(SIZET size, SIZET alignment, const char * file, int line)
{
	ASSERT(T_IS_POWER_OF_2(alignment));
#if defined(__APPLE__)
	void * mem(nullptr);
	VERIFY(posix_memalign(&mem, alignment, size) == 0);
#else
	void * mem = _aligned_malloc(size, alignment);
#endif
	//MEMTRACKER_ALLOC(mem, size, file, line);
	return mem;
}

// Realloc
//------------------------------------------------------------------------------
void * Realloc(void* pMem, SIZET size)
{
	return ReallocFileLine(pMem, size, sizeof(void *), "Unknown", 0);
}

void * Realloc(void* pMem, SIZET size, SIZET alignment)
{
	return ReallocFileLine(pMem, size, alignment, "Unknown", 0);
}

void * ReallocFileLine(void* pMem, SIZET size, const char * file, int line)
{
	return ReallocFileLine(pMem, size, sizeof(void *), file, line);
}

void * ReallocFileLine(void* pMem, SIZET size, SIZET alignment, const char * file, int line)
{
#if defined (__WINDOWS__)
	void * mem = _aligned_realloc(pMem, size, alignment);
	/*if (!T_IS_ALIGN_OF((SIZET)mem, alignment))
	{
		void * alignedMem = AllocFileLine(size, alignment, file, line);
		memcpy(alignedMem, mem, size);
		free(mem);
		mem = alignedMem;
	}*/
#endif

	//MEMTRACKER_ALLOC(mem, size, file, line);
	return mem;
}

// Free
//------------------------------------------------------------------------------
void Free(void * ptr)
{
	//MEMTRACKER_FREE(ptr);

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
