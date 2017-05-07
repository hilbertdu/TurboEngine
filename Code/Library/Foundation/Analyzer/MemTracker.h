// MemTracker.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_MEMORY_MEMTRACKER_H
#define FOUNDATION_MEMORY_MEMTRACKER_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/Platform/Misc.h"
#include "Foundation/Process/Mutex.h"
#include <atomic>
#include <typeinfo>

// Forward Declarations
//------------------------------------------------------------------------------
class MemPoolBlock;

// MemStamp
//------------------------------------------------------------------------------
struct MemStructNone {};

// new operator
//------------------------------------------------------------------------------
void *operator new(SIZET size, const MemStructNone &, const MemStructNone &);
void *operator new[](SIZET size, const MemStructNone &, const MemStructNone &);
void operator delete(void * ptr, const MemStructNone &, const MemStructNone &);
void operator delete[](void * ptr, const MemStructNone &, const MemStructNone &);

// MemStamp
//------------------------------------------------------------------------------
class MemStamp
{
public:
	MemStamp(char const * file, int line)
		: mFile(file)
		, mLine(line) 
	{}
	~MemStamp() {}

	const char* mFile;
	const int	mLine;
};

template<class T> 
inline T *operator*(const MemStamp & stamp, T * p)
{
	MemTracker::TrackStamp(p, stamp, typeid(T).name());
	return p;
}

// MemTracker
//------------------------------------------------------------------------------
class MemTracker
{
public:
	static void * Alloc(SIZET size, SIZET alignment = sizeof(void*));
	static void * Realloc(void * ptr, SIZET size, SIZET alignment = sizeof(void*));
	static void   Free(void * ptr);

	static void TrackAlloc(void * ptr, SIZET size);
	static void TrackFree(void * ptr);

	static void TrackStamp(void *ptr, const MemStamp &stamp, char const *typeName);

	static void Reset();
	static void DumpAllocations();

	static inline uint32 GetCurrentAllocationCount() { return s_AllocationCount; }

	struct Allocation
	{
		void *			m_Ptr;
		size_t			m_Size;
		Allocation *	m_Next;
		const char *	m_File;
		const char *	m_Name;
		uint32			m_Line;
		uint32			m_Id;
	};
private:
	static void Init();

	static Mutex & GetMutex() { return reinterpret_cast<Mutex &>(s_Mutex); }

	static uint32				s_Id;
	static std::atomic<bool>	s_Enabled;
	static std::atomic<bool>	s_Initialized;
	static uint32				s_AllocationCount;
	static Allocation *			s_LastAllocation;
	static uint64				s_Mutex[sizeof(Mutex) / sizeof(uint64)];
	static Allocation **		s_AllocationHashTable;
	static MemPoolBlock *		s_Allocations;
};


//------------------------------------------------------------------------------
#endif // FOUNDATION_MEMORY_MEMTRACKER_H
