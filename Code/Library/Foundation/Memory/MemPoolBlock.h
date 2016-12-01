// MemPoolBlock.h
//------------------------------------------------------------------------------
#pragma once


// Memory pool block illustration
//===================================================
//		 |-------------|----------|----------|
// Page1 |---PagePtr---|--Block1--|--Block2--|.....
//		 |-------------|----------|----------|
//===================================================
//		 |-------------|----------|----------|
// Page2 |---PagePtr---|--Block1--|--Block2--|.....
//		 |-------------|----------|----------|
//===================================================
// PageN...
//===================================================


// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/Platform/Misc.h"
#include "Foundation/Memory/Mem.h"


// Class
//-----------------------------------------------------------------------------
class MemPoolBlock
{
public:
	enum { PAGE_SIZE = 64 * 1024 };

	MemPoolBlock(SIZE_T blockSize, SIZE_T blockAlignment);
	~MemPoolBlock();

	void* Alloc(SIZE_T size);
	void  Free(void* pMem);

	FORCE_INLINE SIZE_T GetBlockSize() const { return m_BlockSize; };
	FORCE_INLINE SIZE_T GetBlockAlignment() const { return m_BlockAlignment; };
	FORCE_INLINE SIZE_T GetAlignedSize() const { return T_MEM_ALIGN_ARB(m_BlockSize, m_BlockAlignment); };

private:
	void AllocPage();

	struct FreeBlock
	{
		FreeBlock * m_Next;
	};

	struct AllocatedPage
	{
		AllocatedPage* m_Next;
	};

	FreeBlock *		m_FreeBlockChain;
	AllocatedPage *	m_AllocatedPageChain;

	SIZE_T m_BlockSize;
	SIZE_T m_BlockAlignment;
	SIZE_T m_PageHeaderSize;

#if T_MEM_STATISTICS
	uint32 m_NumAllocations;
#endif
};