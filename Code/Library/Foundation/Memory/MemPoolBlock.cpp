// MemPoolBlock - Block-based memory allocator
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Memory/MemPoolBlock.h"
#include "Foundation/Memory/MemPool.h"
#include "Foundation/Env/Assert.h"
#include "Foundation/Math/Conversion.h"

#include "Foundation/Logging/Logger.h"

// Constructor
//------------------------------------------------------------------------------
MemPoolBlock::MemPoolBlock(SIZET allocSize, SIZET alignment)
	: m_FreeBlockChain(nullptr)
	, m_AllocatedPageChain(nullptr)
	, m_BlockSize(Math::Max(allocSize, sizeof(FreeBlock)))
	, m_BlockAlignment(alignment)
	, m_PageHeaderSize(T_MEM_ALIGN_ARB(sizeof(AllocatedPage), m_BlockAlignment))
#if T_MEM_STATISTICS
	, m_NumAllocations(0)
#endif
{
	ASSERT(m_BlockSize >= sizeof(FreeBlock));
	ASSERT(m_BlockSize + m_PageHeaderSize <= PAGE_SIZE);
	ASSERT(m_BlockAlignment >= 4);
	ASSERT(m_BlockAlignment <= PAGE_SIZE);

	LOUTPUT("MemPoolBlock constructor: %d (%p)\n", allocSize, this);

	MemPool::InsertMemPoolBlock(this);
}

// Destructor 
//------------------------------------------------------------------------------
MemPoolBlock::~MemPoolBlock()
{
	// Ensure no memory leaks
#if T_MEM_STATISTICS
	ASSERT(m_NumAllocations == 0);
#endif

	LOUTPUT("MemPoolBlock destructor: %p\n", this);

	// Free pages
	AllocatedPage* page = m_AllocatedPageChain;
	AllocatedPage* next = page;
	while (page)
	{
		next = page->m_Next;
		FREE((void *)page);
		page = next;
	}

	// Remove from mem pool list
	MemPool::RemoveMemPoolBlock(this);
}

// Alloc
//------------------------------------------------------------------------------
void * MemPoolBlock::Alloc(SIZET size)
{
	// Only permitted to call with supported sizes
	ASSERT(size <= m_BlockSize); (void)size;

	if (m_FreeBlockChain == nullptr)
	{
		AllocPage();
		ASSERT(m_FreeBlockChain);
	}

#if T_MEM_STATISTICS
	m_NumAllocations++;
#endif

	// Take first block from free chain
	FreeBlock * block = m_FreeBlockChain;
	m_FreeBlockChain = block->m_Next;
	return reinterpret_cast<void *>(block);
}

// Free
//------------------------------------------------------------------------------
void MemPoolBlock::Free(void * pMem)
{
#if T_MEM_STATISTICS
	ASSERT(m_NumAllocations > 0);
#endif

	// Insert free block into head of chain
	FreeBlock * freeBlock = reinterpret_cast<FreeBlock *>(pMem);
	freeBlock->m_Next = m_FreeBlockChain;
	m_FreeBlockChain = freeBlock;

#if T_MEM_STATISTICS
	--m_NumAllocations;
#endif
}

// AlocPage
//------------------------------------------------------------------------------
void MemPoolBlock::AllocPage()
{
	// allocate page from system
	void * newPage = ALLOC(PAGE_SIZE, m_BlockAlignment);

	// sanity check page alignment can support block alignment
	ASSERT(((SIZET)newPage % m_BlockAlignment) == 0);

	// alloc page pointer
	const SIZET alignedSize = T_MEM_ALIGN_ARB(m_BlockSize, m_BlockAlignment);
	const SIZET blockHeader = (SIZET)newPage + alignedSize;

	AllocatedPage* page = m_AllocatedPageChain;
	m_AllocatedPageChain = (AllocatedPage *)newPage;
	m_AllocatedPageChain->m_Next = page;

	// divide page into blocks
	const SIZET numBlocksInPage = (PAGE_SIZE / alignedSize) - 1;

	// build chain into new blocks
	FreeBlock * const firstBlock = reinterpret_cast<FreeBlock *>(blockHeader);
	FreeBlock * block = reinterpret_cast<FreeBlock *>(blockHeader);
	for (SIZET i = 0; i < (numBlocksInPage - 1); ++i)
	{
		FreeBlock * next = reinterpret_cast<FreeBlock *>((SIZET)block + alignedSize);
		block->m_Next = next;
		block = next;
	}

	// hook last block into master chain
	block->m_Next = m_FreeBlockChain;
	m_FreeBlockChain = firstBlock;
}


#if T_MEM_STATISTICS
// SetCategoryName
//------------------------------------------------------------------------------
void MemPoolBlock::SetCategoryName(const char * category)
{
	m_CategoryName = category;
}

// GetUsePercentage
//------------------------------------------------------------------------------
float MemPoolBlock::GetUsePercentage() const
{
	return 0;
}
#endif

//------------------------------------------------------------------------------
