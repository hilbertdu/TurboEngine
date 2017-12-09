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
	BitArray<> bitArr;
	{
	}
}

// ArrayAssignment
//------------------------------------------------------------------------------
void TestBitArray::BitArrayAssignment() const
{
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