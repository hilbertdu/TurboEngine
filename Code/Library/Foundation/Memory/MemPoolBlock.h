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

struct AllocForm;

// Class
//-----------------------------------------------------------------------------
class MemPoolBlock
{
public:
	enum { PAGE_SIZE = 64 * 1024 };

	MemPoolBlock(SIZET allocSize, SIZET alignment);
	~MemPoolBlock();

	void* Alloc(SIZET size);
	void  Free(void* pMem);

	SIZET GetBlockSize() const { return m_BlockSize; }
	SIZET GetBlockAlignment() const { return m_BlockAlignment; }
	SIZET GetAlignedSize() const { return T_MEM_ALIGN_ARB(m_BlockSize, m_BlockAlignment); }

#if T_MEM_STATISTICS
	void  SetCategoryName(const char * category);
	float GetUsePercentage() const;
#endif

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

	SIZET m_BlockSize;
	SIZET m_BlockAlignment;
	SIZET m_PageHeaderSize;

#if T_MEM_STATISTICS
	uint32		m_NumAllocations;
	const char*	m_CategoryName;
#endif
};