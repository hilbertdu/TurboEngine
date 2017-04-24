// MemPool.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_MEMORY_MEMPOOL_H
#define FOUNDATION_MEMORY_MEMPOOL_H


// Memory pool illustration
//================================
// Block size 2 | PoolBlock 1
//================================
// Block size 4 | PoolBlock 2
//================================
// Block size 8 | PoolBlock 3
//================================
// Block size 8 | PoolBlock 4
//================================
// ....
//================================


// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"


// Forward declaration
//------------------------------------------------------------------------------
class MemPoolImpl;
class MemPoolBlock;


// MemPool
//------------------------------------------------------------------------------
class MemPool
{
public:
	static void InsertMemPoolBlock(MemPoolBlock * block);
	static void RemoveMemPoolBlock(MemPoolBlock * block);

private:
	static MemPoolImpl * s_Impl;
};

//------------------------------------------------------------------------------
#endif // FOUNDATION_MEMORY_MEMPOOL_H