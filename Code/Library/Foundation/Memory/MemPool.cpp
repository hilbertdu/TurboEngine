// MemPool.cpp
//------------------------------------------------------------------------------

#include "Foundation/Memory/MemPool.h"
#include "Foundation/Memory/Mem.h"
#include "Foundation/Env/Assert.h"


// Static
//-----------------------------------------------------------------------------
/*static*/ MemPool::PoolBlock* MemPool::s_PoolBlockList = nullptr;

// GetBlockPool
//------------------------------------------------------------------------------
/*static*/ MemPoolBlock* MemPool::GetPoolBlock(SIZE_T blockSize)
{
	MemPoolBlock* poolBlock = FindOrCreate(blockSize);
	ASSERT(poolBlock);
	return poolBlock;
}

// Shrink
//------------------------------------------------------------------------------
/*static*/ void MemPool::Shrink()
{
	// TODO
}

// FindOrCreate
//------------------------------------------------------------------------------
/*static*/ MemPoolBlock* MemPool::FindOrCreate(SIZE_T blockSize)
{
	for (PoolBlock * pb = s_PoolBlockList; pb; pb = pb->m_Next)
	{
		if (pb->m_Block.GetAlignedSize() == blockSize)
		{
			return &pb->m_Block;
		}
	}

	ASSERT(T_IS_ALIGN_OF(blockSize, sizeof(void *)));

	PoolBlock * pb = TNEW(PoolBlock(blockSize, sizeof(void *)));
	PoolBlock * head = s_PoolBlockList;
	s_PoolBlockList = pb;
	s_PoolBlockList->m_Next = head;

	return &pb->m_Block;
}

//------------------------------------------------------------------------------