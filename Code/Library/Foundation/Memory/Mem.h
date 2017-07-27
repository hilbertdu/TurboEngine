// Mem.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_MEMORY_MEM_H
#define FOUNDATION_MEMORY_MEM_H


// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Platform.h"
#include "Foundation/Platform/Types.h"
#include "Foundation/Platform/Misc.h"

// Macros
//------------------------------------------------------------------------------
#define T_MEM_ALIGN_ARB(n, a) ((((UINTPTR)(n)) + ((a)-1)) & ~((a)-1))  // 'a' needs to be a power of 2

#define T_MEM_ALIGN_128(n)    ((((UINTPTR)(n)) + 127) & ~127)
#define T_MEM_ALIGN_64(n)     ((((UINTPTR)(n)) + 63 ) & ~63 )
#define T_MEM_ALIGN_32(n)     ((((UINTPTR)(n)) + 31 ) & ~31 )
#define T_MEM_ALIGN_16(n)     ((((UINTPTR)(n)) + 15 ) & ~15 )
#define T_MEM_ALIGN_8(n)      ((((UINTPTR)(n)) + 7  ) & ~7  )
#define T_MEM_ALIGN_4(n)      ((((UINTPTR)(n)) + 3  ) & ~3  )
#define T_MEM_ALIGN_2(n)      ((((UINTPTR)(n)) + 1  ) & ~1  )

#define T_IS_POWER_OF_2(n)    (((n) != 0) && !((n) & (n - 1)))
#define T_IS_ALIGN_OF(n, a)   (T_MEM_ALIGN_ARB(n, a) == n)

#define T_MEM_DEFAULT_ALIGN    sizeof(void*)

#define T_MEM_STATISTICS	1
#define T_MEM_TRACKER		1
#define T_MEM_TRACKER_ALL	1

// new/delete
//------------------------------------------------------------------------------
#define INPLACE_NEW			new

#if (T_MEM_TRACKER)
	#include "Foundation/Analyzer/MemTracker.h"

	#if (T_MEM_TRACKER_ALL)
		#define new							MemStamp(__FILE__, __LINE__) * new
		#define TNEW(code)					new code
		#define TNEW_ARRAY(code)			new code
		#define TDELETE						delete
		#define TDELETE_ARRAY				delete[]
		#define TDELETE_SAFE(code)			do { if (code) { delete code; } } while(0)
		#define TDELETE_ARRAY_SAFE(code)	do { if (code) { delete[] code; } } while(0)
	#else
		#define TNEW(code)			(MemStamp(__FILE__, __LINE__) * new(MemStructNone(), MemStructNone()) code)
		#define TNEW_ARRAY(code)	(MemStamp(__FILE__, __LINE__) * new(MemStructNone(), MemStructNone()) code)
		#define TDELETE				delete
		#define TDELETE_ARRAY		delete[]
	#endif

	#define ALLOC(...)			(MemStamp(__FILE__, __LINE__) * MemTracker::Alloc(__VA_ARGS__))
	#define REALLOC(...)		(MemStamp(__FILE__, __LINE__) * MemTracker::Realloc(__VA_ARGS__))
	#define FREE(ptr)			MemTracker::Free(ptr)

	#define TMEM_TRACKER_DUMP_INFO	MemTracker::DumpAllocations();
#else
	#define TNEW(code)			new code
	#define TNEW_ARRAY(code)	new code
	#define TDELETE				delete
	#define TDELETE_ARRAY		delete[]

	#define ALLOC(...)			::Alloc(__VA_ARGS__)
	#define REALLOC(...)		::Realloc(__VA_ARGS__)
	#define FREE(ptr)			::Free(ptr)
#endif


// Alloc/Free
//------------------------------------------------------------------------------
void * Alloc(SIZET size, SIZET alignment = T_MEM_DEFAULT_ALIGN);
void * Realloc(void* pMem, SIZET size, SIZET alignment = T_MEM_DEFAULT_ALIGN);
void Free(void * ptr);

// Copy/Move/Set
//------------------------------------------------------------------------------
void MemCopy(void* des, const void* src, SIZET size);
void MemMove(void* des, const void* src, SIZET size);
void MemSet(void* des, int value, SIZET size);


//------------------------------------------------------------------------------
#endif // FOUNDATION_MEMORY_MEM_H