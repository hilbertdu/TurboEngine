// Mem.h
//------------------------------------------------------------------------------
#pragma once


// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Platform.h"
#include "Foundation/Platform/Types.h"


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


#define T_MEM_STATISTICS		1
#define T_MEM_TRACKER_ENABLE	0


// new/delete
//------------------------------------------------------------------------------
#if (T_MEM_TRACKER_ENABLE)
	#define TNEW( code )		new ( __FILE__, __LINE__ ) code
	#define TNEW_ARRAY( code )	new ( __FILE__, __LINE__ ) code
	#define TDELETE				delete
	#define TDELETE_ARRAY		delete[]

	#define ALLOC( ... )		::AllocFileLine( __VA_ARGS__, __FILE__, __LINE__ )
	#define FREE( ptr )			::Free( ptr )
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
void * Alloc(SIZET size);
void * Alloc(SIZET size, SIZET alignment);
void * AllocFileLine(SIZET size, const char * file, int line);
void * AllocFileLine(SIZET size, SIZET alignment, const char * file, int line);
void * Realloc(void* pMem, SIZET size);
void * Realloc(void* pMem, SIZET size, SIZET alignment);
void * ReallocFileLine(void* pMem, SIZET size, const char * file, int line);
void * ReallocFileLine(void* pMem, SIZET size, SIZET alignment, const char * file, int line);
void Free(void * ptr);

// Copy/Move/Set
//------------------------------------------------------------------------------
void MemCopy(void* des, const void* src, SIZET size);
void MemMove(void* des, const void* src, SIZET size);
void MemSet(void* des, int value, SIZET size);


// Placement new/delete
//------------------------------------------------------------------------------
#define INPLACE_NEW new
#include <new>
//inline void * operator new(SIZET, void * ptr){ return ptr; }
//inline void * operator new[](SIZET, void * ptr) { return ptr; }
//inline void operator delete(void *, void *) {}
//inline void operator delete[](void *, void *) {}


// Global new/delete
//------------------------------------------------------------------------------
//inline void * operator new(SIZET size){ return Alloc(size); }
//inline void * operator new[](SIZET size) { return Alloc(size); }
//inline void operator delete(void * ptr) { Free(ptr); }
//inline void operator delete[](void * ptr) { Free(ptr); }


//------------------------------------------------------------------------------