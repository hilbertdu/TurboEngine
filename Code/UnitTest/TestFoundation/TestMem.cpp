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
	void TestMemTracker() const;
};

// Register Tests
//------------------------------------------------------------------------------
REGISTER_TESTS_BEGIN(TestMem)
	REGISTER_TEST(TestUnused)
	REGISTER_TEST(TestAllocs)
	REGISTER_TEST(TestAllocsMultiplePages);
	REGISTER_TEST(TestSpeed);
	REGISTER_TEST(TestMemPool);
	REGISTER_TEST(TestMemTracker);
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
	constexpr size_t blockSize(32);
	constexpr size_t blockAlignment(4);
	//MemPoolBlock block(blockSize, blockAlignment);

	// Allocate every size up to the block size
	Array<void *> allocs(blockSize + 1);
	PoolAllocator<TAllocForm<blockSize, blockAlignment>> allocator;
	for (size_t i = 0; i <= blockSize; ++i)
	{
		//void * mem = block.Alloc(i);
		//void * mem = allocator.Allocate(i);
		void * mem = allocator.AllocateT<char>(i);
		TEST_ASSERT(mem);
		TEST_ASSERT(((size_t)mem % blockAlignment) == 0);
		allocs[i] = mem;
	}

	// Free them
	for (size_t i = 0; i < allocs.GetSize(); ++i)
	{
		allocator.Free(allocs[i]);
		//block.Free(allocs[i]);
	}
}

// TestAllocsMultiplePages
//------------------------------------------------------------------------------
void TestMem::TestAllocsMultiplePages() const
{
	constexpr size_t blockSize(32 * 1024);
	constexpr size_t blockAlignment(4);
	//MemPoolBlock block(blockSize, blockAlignment);
	PoolAllocator<TAllocForm<blockSize, blockAlignment>> allocator;

	void * a = allocator.Allocate(1); // 32 KiB
	void * b = allocator.Allocate(1); // 32 KiB
	void * c = allocator.Allocate(1); // 32 KiB, new page
	allocator.Free(a);
	allocator.Free(b);
	allocator.Free(c);
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
	PoolAllocator<TAllocForm<32>> poolAllocator1;
	PoolAllocator<TAllocForm<32, 8>> poolAllocator2;
	PoolAllocator<TAllocForm<24, 16>> poolAllocator3;
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
	{
		using TAllocFormInt32 = TAllocForm<sizeof(int32)>;

		auto p1 = PoolAllocator<TAllocFormInt32>();
		auto p2 = PoolAllocator<TAllocFormInt32, CONSTSTR("Category1")>();

		Array<int32, PoolAllocator<TAllocForm<sizeof(int32)>>> arr1;
		Array<int32, decltype(p2)> arr2;
	}
	{
		PoolAllocator<TAllocForm<32>> allocator = poolAllocator1;
		void* mem = allocator.Allocate(32);
		poolAllocator1.Free(mem);
	}
}

// TestMemTracker
//------------------------------------------------------------------------------
void TestMem::TestMemTracker() const
{
#if defined(TMEM_TRACKER_DUMP_INFO)
	TMEM_TRACKER_DUMP_INFO
#endif
}

//------------------------------------------------------------------------------
