// MemTracker.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "MemTracker.h"

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Memory/MemPoolBlock.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/Env/Assert.h"
#include "Foundation/Math/Conversion.h"
#include "Foundation/Memory/Mem.h"
#include <thread>


// Static Data
//------------------------------------------------------------------------------
/*static*/ uint32				MemTracker::s_Id(0);
/*static*/ std::atomic<bool>	MemTracker::s_Enabled(true);
/*static*/ std::atomic<bool>	MemTracker::s_Initialized(false);
/*static*/ uint32				MemTracker::s_AllocationCount(0);
/*static*/ uint64				MemTracker::s_Mutex[];
/*static*/ MemTracker::Allocation ** MemTracker::s_AllocationHashTable = nullptr;
/*static*/ MemPoolBlock *		MemTracker::s_Allocations(nullptr);

// Thread-Local Data
//------------------------------------------------------------------------------
thread_local uint32 g_MemTrackerDisabled(0);

// Defines
//------------------------------------------------------------------------------
#define ALLOCATION_MINIMUM_ALIGN	( 0x4 )			// assume at least 4 byte alignment
#define ALLOCATION_HASH_SHIFT		( 0x2 )			// shift off lower bits
#define ALLOCATION_HASH_SIZE		( 0x100000 )	// one megabyte
#define ALLOCATION_HASH_MASK		( 0x0FFFFF )	// mask off upper bits


// Alloc
//------------------------------------------------------------------------------
/*static*/ void * MemTracker::Alloc(SIZET size, SIZET alignment)
{
	void * p = ::Alloc(size, alignment);
	TrackAlloc(p, size);
	return p;
}

// Realloc
//------------------------------------------------------------------------------
/*static*/ void * MemTracker::Realloc(void * ptr, SIZET size, SIZET alignment)
{
	void * p = ::Realloc(ptr, size, alignment);
	TrackAlloc(p, size);
	return p;
}

// Free
//------------------------------------------------------------------------------
/*static*/ void MemTracker::Free(void * ptr)
{
	TrackFree(ptr);
	::Free(ptr);
}

// TrackAlloc
//------------------------------------------------------------------------------
/*static*/ void MemTracker::TrackAlloc(void * ptr, SIZET size)
{
	if (!s_Enabled) {
		return;
	}

	if (g_MemTrackerDisabled)
	{
		return;
	}

	++g_MemTrackerDisabled;

	if (!s_Initialized)
	{
		Init();
	}

	const SIZET hashIndex = (((SIZET)ptr >> ALLOCATION_HASH_SHIFT) & ALLOCATION_HASH_MASK);

	{
		LockGuard<MutexLock> mh(GetMutex());

		Allocation * a = (Allocation *)s_Allocations->Alloc(sizeof(Allocation));
		++s_AllocationCount;

		a->m_Id = ++s_Id;
		a->m_Ptr = ptr;
		a->m_Size = size;
		a->m_Next = s_AllocationHashTable[hashIndex];
		//a->m_File = file;
		//a->m_Line = line;
		static SIZET breakOnSize = (SIZET)-1;
		static uint32 breakOnId = 0;
		if ((size == breakOnSize) || (a->m_Id == breakOnId))
		{
			BREAK_IN_DEBUGGER;
		}
		s_AllocationHashTable[hashIndex] = a;
	}
	--g_MemTrackerDisabled;
}

// Free
//------------------------------------------------------------------------------
/*static*/ void MemTracker::TrackFree(void * ptr)
{
	if (!s_Enabled || !s_Initialized)
	{
		return;
	}

	if (g_MemTrackerDisabled)
	{
		return;
	}

	const SIZET hashIndex = (((SIZET)ptr >> ALLOCATION_HASH_SHIFT) & ALLOCATION_HASH_MASK);

	LockGuard<MutexLock> mh(GetMutex());

	Allocation * a = s_AllocationHashTable[hashIndex];
	Allocation * prev = nullptr;
	while (a)
	{
		if (a->m_Ptr == ptr)
		{
			if (prev == nullptr)
			{
				s_AllocationHashTable[hashIndex] = a->m_Next;
			}
			else
			{
				prev->m_Next = a->m_Next;
			}
			++g_MemTrackerDisabled;
			s_Allocations->Free(a);
			--s_AllocationCount;
			--g_MemTrackerDisabled;
			break;
		}
		prev = a;
		a = a->m_Next;
	}
}

// TrackStamp
//------------------------------------------------------------------------------
/*static*/ void MemTracker::TrackStamp(void *ptr, const MemStamp &stamp, char const *typeName)
{
	if (!s_Enabled || !s_Initialized) {
		return;
	}

	const SIZET hashIndex = (((SIZET)ptr >> ALLOCATION_HASH_SHIFT) & ALLOCATION_HASH_MASK);

	Allocation * a = s_AllocationHashTable[hashIndex];
	if (a && a->m_Ptr == ptr)
	{
		a->m_File = stamp.mFile;
		a->m_Line = stamp.mLine;
		a->m_Name = typeName;
	}
}

// DumpAllocations
//------------------------------------------------------------------------------
/*static*/ void MemTracker::DumpAllocations()
{
	if (s_Enabled == false)
	{
		LINFO("MemTracker", "DumpAllocations failed - MemTracker not enabled\n");
		return;
	}

	if (s_Initialized == false)
	{
		LINFO("MemTracker", "DumpAllocations : No allocations\n");
		return;
	}

	LockGuard<MutexLock> mh(GetMutex());

	if (s_AllocationCount == 0)
	{
		LINFO("MemTracker", "DumpAllocations : No allocations\n");
		return;
	}

	uint64 total = 0;
	uint64 numAllocs = 0;

	// for each leak, we'll print a view of the memory
	unsigned char displayChar[256];
	MemSet(displayChar, '.', sizeof(displayChar));
	const unsigned char * okChars = (const unsigned char *)"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ~`1234567890-=!@#$^&*()_+[]{};:'\",<>/?|\\";
	const unsigned char * ok = okChars;
	for (;; ) { unsigned char c = *ok; if (c == 0) break; displayChar[c] = c; ++ok; }

	char memView[32] = { 0 };

	LINFO("MemTracker", "--- DumpAllocations ------------------------------------------------\n");
	for (size_t i = 0; i < ALLOCATION_HASH_SIZE; ++i)
	{
		Allocation * a = s_AllocationHashTable[i];
		while (a)
		{
			uint32 id = a->m_Id;
			uint64 addr = (size_t)a->m_Ptr;
			uint64 size = a->m_Size;

			// format a view of the memory contents
			const char * src = (const char *)addr;
			char * dst = memView;
			const size_t num = Math::Min< size_t >((size_t)size, 31);
			for (uint32 j = 0; j < num; ++j)
			{
				unsigned char c = *src;
				*dst = displayChar[c];
				++src;
				++dst;
			}
			*dst = 0;

			LINFO("MemTracker", "%s(%u): Id %u : %llu bytes @ 0x%016llx (Type: %s, Mem: %s)\n", a->m_File, a->m_Line, id, size, addr, a->m_Name, memView);

			++numAllocs;
			total += size;

			a = a->m_Next;
		}
	}
	LINFO("MemTracker", "--------------------------------------------------------------------\n");
	LINFO("MemTracker", "Total: %llu bytes in %llu allocs\n", total, numAllocs);
	LINFO("MemTracker", "--------------------------------------------------------------------\n");
}

// Reset
//------------------------------------------------------------------------------
/*static*/ void MemTracker::Reset()
{
	LockGuard<MutexLock> mh(GetMutex());

	// free all allocation tracking
	for (size_t i = 0; i < ALLOCATION_HASH_SIZE; ++i)
	{
		Allocation * a = s_AllocationHashTable[i];
		while (a)
		{
			s_Allocations->Free(a);
			--s_AllocationCount;
			a = a->m_Next;
		}
		s_AllocationHashTable[i] = nullptr;
	}

	ASSERT(s_AllocationCount == 0);

	s_Id = 0;
}

// Init
//------------------------------------------------------------------------------
/*static*/ void MemTracker::Init()
{
	CASSERT(sizeof(MemTracker::s_Mutex) == sizeof(MutexLock));

	static std::atomic_flag threadSafeGuard = ATOMIC_FLAG_INIT;
	while (threadSafeGuard.test_and_set())
	{
		while (s_Initialized) { std::this_thread::yield(); }
		return;
	}

	// construct primary mutex in-place
	INPLACE_NEW(&GetMutex()) MutexLock;

	// init hash table
	s_AllocationHashTable = new Allocation*[ALLOCATION_HASH_SIZE];
	MemSet(s_AllocationHashTable, 0, ALLOCATION_HASH_SIZE * sizeof(Allocation *));

	// init pool for allocation structures
	s_Allocations = new MemPoolBlock(sizeof(Allocation), __alignof(Allocation));

	s_Initialized = true;
}


// Operators
//------------------------------------------------------------------------------
#if (T_MEM_TRACKER)
#pragma push_macro("new")
#undef new
	#if (T_MEM_TRACKER_ALL)
		void *operator new(SIZET size)
		{
			return MemTracker::Alloc(size);
		}

		void operator delete(void *p)
		{
			MemTracker::Free(p);
		}

		void *operator new[](SIZET size)
		{
			return MemTracker::Alloc(size);
		}

		void operator delete[](void *p)
		{
			MemTracker::Free(p);
		}
	#else
		void *operator new(SIZET size, const MemStructNone &, const MemStructNone &)
		{
			return MemTracker::Alloc(size);
		}

		void operator delete(void *p, const MemStructNone &, const MemStructNone &)
		{
			MemTracker::Free(p);
		}

		void *operator new[](SIZET size, const MemStructNone &, const MemStructNone &)
		{
			return MemTracker::Alloc(size);
		}

		void operator delete[](void *p, const MemStructNone &, const MemStructNone &)
		{
			MemTracker::Free(p);
		}
	#endif
#pragma pop_macro("new")
#endif

//------------------------------------------------------------------------------
