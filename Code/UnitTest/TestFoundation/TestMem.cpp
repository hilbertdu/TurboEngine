// TestMemPoolBlock.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "TestFramework/UnitTest.h"

#include "Foundation/Platform/Types.h"
#include "Foundation/Memory/Mem.h"
#include "Foundation/Memory/MemPoolBlock.h"
#include "Foundation/Memory/Allocator.h"
#include "Foundation/Time/Timer.h"
#include "Foundation/Logging/Logger.h"


// TestMemPoolBlock
//------------------------------------------------------------------------------
class TestMem : public UnitTest
{
private:
	DECLARE_TESTS

	void TestUnused() const;
	void TestAllocs() const;
	void TestAllocsMultiplePages() const;
	void TestSpeed();
	void TestMemPool() const;
};

// Register Tests
//------------------------------------------------------------------------------
REGISTER_TESTS_BEGIN(TestMem)
	REGISTER_TEST(TestUnused)
	REGISTER_TEST(TestAllocs)
	REGISTER_TEST(TestAllocsMultiplePages);
	REGISTER_TEST(TestSpeed);
	REGISTER_TEST(TestMemPool);
REGISTER_TESTS_END

// TestUnused
//------------------------------------------------------------------------------
void TestMem::TestUnused() const
{
	// Create a MemPoolBlock but don't do anything with it
	MemPoolBlock block(32, 4);
}

// TestAllocs
//------------------------------------------------------------------------------
void TestMem::TestAllocs() const
{
	const size_t blockSize(32);
	const size_t blockAlignment(4);
	MemPoolBlock block(blockSize, blockAlignment);

	// Allocate every size up to the block size
	Array<void *> allocs(blockSize + 1);
	for (size_t i = 0; i <= blockSize; ++i)
	{
		void * mem = block.Alloc(i);
		TEST_ASSERT(mem);
		TEST_ASSERT(((size_t)mem % blockAlignment) == 0);
		allocs[i] = mem;
	}

	// Free them
	for (size_t i = 0; i < allocs.GetSize(); ++i)
	{
		block.Free(allocs[i]);
	}
}

// TestAllocsMultiplePages
//------------------------------------------------------------------------------
void TestMem::TestAllocsMultiplePages() const
{
	const size_t blockSize(32 * 1024);
	const size_t blockAlignment(4);
	MemPoolBlock block(blockSize, blockAlignment);

	void * a = block.Alloc(1); // 32 KiB
	void * b = block.Alloc(1); // 32 KiB
	void * c = block.Alloc(1); // 32 KiB, new page
	block.Free(a);
	block.Free(b);
	block.Free(c);
}

// TestSpeed
//------------------------------------------------------------------------------
void TestMem::TestSpeed()
{
	const uint32 numAllocs(20 * 1000);
	const uint32 allocSize(24);

	float time1(0.0f);
	float time2(0.0f);

	// Alloc
	{
		Array<void *> allocs(numAllocs);
		Timer t1;
		{
			for (uint32 i = 0; i < numAllocs; ++i)
			{
				uint32 * mem = (uint32 *)ALLOC(allocSize);
				allocs[i] = mem;
			}
			for (uint32 i = 0; i < numAllocs; ++i)
			{
				void * mem = allocs[i];
				FREE(mem);
			}
		}
		time1 = t1.GetElapsed();
	}

	// MemPoolBlock
	{
		Array<void *> allocs(numAllocs);
		Timer t2;
		{
			MemPoolBlock block(allocSize, 4);
			for (uint32 i = 0; i < numAllocs; ++i)
			{
				uint32 * mem = (uint32 *)block.Alloc(allocSize);
				allocs[i] = mem;
			}
			for (uint32 i = 0; i < numAllocs; ++i)
			{
				void * mem = allocs[i];
				block.Free(mem);
			}
		}
		time2 = t2.GetElapsed();
	}

	// output
	LOUTPUT("Alloc        : %2.3fs - %u allocs @ %u allocs/sec\n", time1, numAllocs, (uint32)((float)numAllocs / time1));
	LOUTPUT("MemPoolBlock : %2.3fs - %u allocs @ %u allocs/sec\n", time2, numAllocs, (uint32)((float)numAllocs / time2));
}

// TestMemPool
//------------------------------------------------------------------------------
void TestMem::TestMemPool() const
{
	PoolAllocator<32, 4> poolAllocator1;
	PoolAllocator<32, 8> poolAllocator2;
	PoolAllocator<24, 16> poolAllocator3;
	TEST_ASSERT(poolAllocator1.GetPoolBlock() == poolAllocator2.GetPoolBlock());
	TEST_ASSERT(poolAllocator1.GetPoolBlock() == poolAllocator3.GetPoolBlock());

	{
		void* mem1 = poolAllocator1.Allocate(1);
		void* mem2 = poolAllocator1.Allocate(16);
		void* mem3 = poolAllocator2.Allocate(31);
		void* mem4 = poolAllocator2.Allocate(32);

		poolAllocator1.Free(mem1);
		poolAllocator1.Free(mem2);
		poolAllocator2.Free(mem3);
		poolAllocator2.Free(mem4);
	}
}

//------------------------------------------------------------------------------
