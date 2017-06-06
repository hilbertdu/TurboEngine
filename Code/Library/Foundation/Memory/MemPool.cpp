// MemPool.cpp
//------------------------------------------------------------------------------

#include "Foundation/Memory/MemPool.h"
#include "Foundation/Memory/MemPoolBlock.h"
#include "Foundation/Container/Array.h"
#include "Foundation/Env/Assert.h"
//#include <algorithm>


// Macros
//------------------------------------------------------------------------------
#define TMEMPOOL_MAX_NUM	32


class MemPoolImpl
{
public:
	using ArrayBlockType = StackArray<MemPoolBlock *, TMEMPOOL_MAX_NUM>;

	MemPoolImpl(): m_MemPoolBlocks(ArrayBlockType(TMEMPOOL_MAX_NUM)) {}

	void InsertMemPoolBlock(MemPoolBlock * block)
	{
		ASSERT(!m_MemPoolBlocks.Find(block));
		m_MemPoolBlocks.Append(block);
	}

	void RemoveMemPoolBlock(MemPoolBlock * block)
	{
		ASSERT(m_MemPoolBlocks.Find(block));
		m_MemPoolBlocks.EraseSwap(m_MemPoolBlocks.Find(block));
	}

private:
	ArrayBlockType m_MemPoolBlocks;
};


// Statics
//------------------------------------------------------------------------------
/*static*/ MemPoolImpl* MemPool::s_Impl(nullptr);


// InsertMemPoolBlock
//------------------------------------------------------------------------------
/*static*/ void MemPool::InsertMemPoolBlock(MemPoolBlock * block)
{
	if (!s_Impl)
	{
		s_Impl = TNEW(MemPoolImpl);
	}
	s_Impl->InsertMemPoolBlock(block);
}

// RemoveMemPoolBlock
//------------------------------------------------------------------------------
/*static*/ void MemPool::RemoveMemPoolBlock(MemPoolBlock * block)
{
	if (!s_Impl)
	{
		s_Impl = TNEW(MemPoolImpl);
	}
	s_Impl->RemoveMemPoolBlock(block);
}

//------------------------------------------------------------------------------