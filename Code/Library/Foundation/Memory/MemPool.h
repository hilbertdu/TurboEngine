// MemPool.h
//------------------------------------------------------------------------------
#pragma once


// Memory pool illustration
//================================
// Block size 2 | PoolBlock
//================================
// Block size 4 | PoolBlock
//================================
// Block size 8 | PoolBlock
//================================
// Block size 8 | PoolBlock
//================================
// ....
//================================


#include "Foundation/Platform/Types.h"
#include "Foundation/Memory/MemPoolBlock.h"


class MemPool
{
public:
	static MemPoolBlock* GetPoolBlock(SIZE_T blockSize);
	static void          Shrink();

private:
	struct PoolBlock
	{
	public:
		PoolBlock(SIZE_T blockSize, SIZE_T alignment) : m_Block(blockSize, alignment) {};

		MemPoolBlock m_Block;
		PoolBlock *  m_Next;
	};

	static MemPoolBlock* FindOrCreate(SIZE_T blockSize);

	static PoolBlock* s_PoolBlockList;
};
