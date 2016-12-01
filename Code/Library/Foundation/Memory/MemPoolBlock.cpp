// MemPoolBlock - Block-based memory allocator
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Mem.h"
#include "MemPoolBlock.h"
#include "Foundation/Env/Assert.h"

// Constructor
//------------------------------------------------------------------------------
MemPoolBlock::MemPoolBlock(SIZE_T blockSize, SIZE_T blockAlignment)
	: m_FreeBlockChain(nullptr)
	, m_AllocatedPageChain(nullptr)
	, m_BlockSize(blockSize)
	, m_BlockAlignment(blockAlignment)
	, m_PageHeaderSize(T_MEM_ALIGN_ARB(sizeof(AllocatedPage), m_BlockAlignment))
#ifdef T_MEM_STATISTICS
	, m_NumAllocations(0)
#endif
{
	ASSERT(blockSize >= sizeof(FreeBlock));
	ASSERT(blockSize + m_PageHeaderSize <= PAGE_SIZE);
	ASSERT(blockAlignment >= 4);
	ASSERT(blockAlignment <= PAGE_SIZE);
}

// Destructor 
//------------------------------------------------------------------------------
MemPoolBlock::~MemPoolBlock()
{
	// Ensure no memory leaks
#if T_MEM_STATISTICS
	ASSERT(m_NumAllocations == 0);
#endif

	// Free pages
	AllocatedPage* page = m_AllocatedPageChain;
	AllocatedPage* next = page;
	while (page)
	{
		next = page->m_Next;
		FREE((void *)page);
		page = next;
	}
}

// Alloc
//------------------------------------------------------------------------------
void * MemPoolBlock::Alloc(SIZE_T size)
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
	ASSERT(((SIZE_T)newPage % m_BlockAlignment) == 0);

	// alloc page pointer
	const SIZE_T alignedSize = T_MEM_ALIGN_ARB(m_BlockSize, m_BlockAlignment);
	const SIZE_T blockHeader = (SIZE_T)newPage + alignedSize;

	AllocatedPage* page = m_AllocatedPageChain;
	m_AllocatedPageChain = (AllocatedPage *)newPage;
	m_AllocatedPageChain->m_Next = page;

	// divide page into blocks
	const SIZE_T numBlocksInPage = (PAGE_SIZE / alignedSize) - 1;

	// build chain into new blocks
	FreeBlock * const firstBlock = reinterpret_cast<FreeBlock *>(blockHeader);
	FreeBlock * block = reinterpret_cast<FreeBlock *>(blockHeader);
	for (SIZE_T i = 0; i < (numBlocksInPage - 1); ++i)
	{
		FreeBlock * next = reinterpret_cast<FreeBlock *>((SIZE_T)block + alignedSize);
		block->m_Next = next;
		block = next;
	}

	// hook last block into master chain
	block->m_Next = m_FreeBlockChain;
	m_FreeBlockChain = firstBlock;
}

//------------------------------------------------------------------------------
