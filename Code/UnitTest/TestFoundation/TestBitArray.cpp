// TestArray.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "TestFramework/UnitTest.h"

#include "Foundation/Container/BitArray.h"
#include "Foundation/Memory/Allocator.h"

// TestString
//------------------------------------------------------------------------------
class TestBitArray : public UnitTest
{
private:
	DECLARE_TESTS

	void BitArrayConstructors() const;
	void BitArrayAssignment() const;
	void BitArrayIterator() const;
	void BitArrayAllocator() const;
};

// Register Tests
//------------------------------------------------------------------------------
REGISTER_TESTS_BEGIN(TestBitArray)
	REGISTER_TEST(BitArrayConstructors)
	REGISTER_TEST(BitArrayAssignment)
	REGISTER_TEST(BitArrayIterator)
	REGISTER_TEST(BitArrayAllocator)
REGISTER_TESTS_END


// ArrayConstructors
//------------------------------------------------------------------------------
void TestBitArray::BitArrayConstructors() const
{
	{
		BitArray<> bitArr1;
		BitArray<> bitArr2(100);
		TEST_ASSERT(bitArr1.GetSize() == 0);
		TEST_ASSERT(bitArr2.GetSize() == 100);
	}
}

// ArrayAssignment
//------------------------------------------------------------------------------
void TestBitArray::BitArrayAssignment() const
{
	BitArray<> bitArr(8);
	{
		bitArr.SetAll(false);
		bitArr.Set(0, true);
		bitArr.Set(7, true);
		TEST_ASSERT(bitArr[0] == true);
		TEST_ASSERT(bitArr[7] == true);
		TEST_ASSERT(bitArr[1] == false);

		bitArr.SetAll(true);
		TEST_ASSERT(bitArr[1] == true);
	}
	{
		bitArr.SetAll(false);

	}
}

// ArrayIterator
//------------------------------------------------------------------------------
void TestBitArray::BitArrayIterator() const
{
}

// ArrayAllocator
//------------------------------------------------------------------------------
void TestBitArray::BitArrayAllocator() const
{
}

//------------------------------------------------------------------------------