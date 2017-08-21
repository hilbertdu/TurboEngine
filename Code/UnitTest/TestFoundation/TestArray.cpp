// TestMap.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "TestFramework/UnitTest.h"

#include "Foundation/Container/Array.h"
#include "Foundation/String/String.h"
#include "Foundation/Memory/Allocator.h"

// TestString
//------------------------------------------------------------------------------
class TestArray : public UnitTest
{
private:
	DECLARE_TESTS

	void ArrayConstructors() const;
	void ArrayAssignment() const;
	void ArrayInsertErase() const;
	void ArrayIterator() const;
	void ArrayAllocator() const;
};

// Register Tests
//------------------------------------------------------------------------------
REGISTER_TESTS_BEGIN(TestArray)
	REGISTER_TEST(ArrayConstructors)
	REGISTER_TEST(ArrayAssignment)
	REGISTER_TEST(ArrayInsertErase)
	REGISTER_TEST(ArrayIterator)
	REGISTER_TEST(ArrayAllocator)
REGISTER_TESTS_END


// ArrayConstructors
//------------------------------------------------------------------------------
void TestArray::ArrayConstructors() const
{
	Array<int> arr;
	{
		Array<int> arr1, arr2(1);
		TEST_ASSERT(arr1.GetSize() == 0);
		TEST_ASSERT(arr1.GetCapacity() == 0);
		TEST_ASSERT(arr2.GetSize() == 1);
		TEST_ASSERT(arr2.GetCapacity() == 1);
	}
	{
		Array<int> arr1 = arr;
		TEST_ASSERT(arr1.GetSize() == 0);
		TEST_ASSERT(arr1.GetCapacity() == 0);
	}
	{
		Array<int> arr1;
		arr1 = arr;
		TEST_ASSERT(arr1.GetSize() == 0);
		TEST_ASSERT(arr1.GetCapacity() == 0);
	}
	{
		const int raw[2] = { 1, 2 };
		Array<int> arr1(raw, raw + 2);
		TEST_ASSERT(arr1.GetSize() == 2);
		TEST_ASSERT(arr1.GetCapacity() >= 2);
		TEST_ASSERT(arr1[0] == 1 && arr1[1] == 2);
	}
}

// ArrayAssignment
//------------------------------------------------------------------------------
void TestArray::ArrayAssignment() const
{
	{
		Array<AString> arr1;
		TEST_ASSERT(arr1.IsEmpty());
		arr1.Insert((size_t)0, AString("test0"));
		arr1.Insert((size_t)1, AString("test1"));
		TEST_ASSERT(arr1[0] == "test0");
		TEST_ASSERT(arr1[1] == "test1");
		TEST_ASSERT(!arr1.IsEmpty());
	}
	{
		Array<AString> arr1;
		arr1.SetSize(10);
		for (int idx = 0; idx < 10; ++idx)
		{
			arr1[idx] = AString("1234");
		}
		TEST_ASSERT(arr1.GetSize() == 10);
		TEST_ASSERT(arr1.GetCapacity() >= 10);

		arr1.SetCapacity(20);
		TEST_ASSERT(arr1.GetCapacity() == 20);
	}
	{
		Array<AString> arr1(10);
		for (int idx = 0; idx < 10; ++idx)
		{
			arr1[idx] = AString("1234");
		}
		Array<AString> arr2;
		arr2.Append(AString("100"));
		arr2 = arr1;
		TEST_ASSERT(arr2[0] == "1234");
	}
}

// ArrayInsertErase
//------------------------------------------------------------------------------
#include <vector>
void TestArray::ArrayInsertErase() const
{
	Array<AString> arr1;
	{
		arr1.Insert((size_t)0, AString("test1"));
		arr1.Insert((size_t)0, AString("test0"));
		TEST_ASSERT(arr1[0] == "test0");
		TEST_ASSERT(arr1[1] == "test1");
	}
	{
		arr1.Pop();
		TEST_ASSERT(arr1[0] == "test0");
		TEST_ASSERT(arr1.GetSize() == 1);
	}
	{
		arr1.Insert(1, AString("test1"));
		arr1.PopFront();
		TEST_ASSERT(arr1[0] == "test1");
		TEST_ASSERT(arr1.GetSize() == 1);
	}
	{
		arr1.EraseIndex(0);
		TEST_ASSERT(arr1.GetSize() == 0);
	}
	{
		arr1.Append(AString("append0"));
		arr1.Append(AString("append1"));
		TEST_ASSERT(arr1.GetSize() == 2);
	}
	{
		arr1.Clear();
		TEST_ASSERT(arr1.GetSize() == 0);
	}
	{
		arr1.EmplaceAppend("test_emplace1");
		arr1.EmplaceAppend("test_emplace2");
		TEST_ASSERT(arr1[0] == "test_emplace1");
		TEST_ASSERT(arr1[1] == "test_emplace2");
	}
	{
		arr1.EmplaceInsert(arr1.Begin(), "test_emplace0");
		TEST_ASSERT(arr1[0] == "test_emplace0");
	}
	{
		Array<int64> arr;
		Timer t;
		for (int64 idx = 0; idx < 100000; ++idx)
		{
			arr.Append(idx);
		}
		LOUTPUT("Array : %2.4fs\n", t.GetElapsed());
	}
	{
		std::vector<int64> arr;
		Timer t;
		for (int64 idx = 0; idx < 100000; ++idx)
		{
			arr.push_back(idx);
		}
		LOUTPUT("std::vector : %2.4fs\n", t.GetElapsed());
	}
}

// ArrayIterator
//------------------------------------------------------------------------------
void TestArray::ArrayIterator() const
{
	Array<AString> arr(10);
	{
		for (Array<AString>::Iter iter = arr.Begin(); iter != arr.End(); ++iter)
		{
			(*iter) = AString("iterator");
		}
	}
	{
		arr.Insert(arr.Begin(), AString("insert_start"));
		arr.Insert(arr.End(), AString("insert_end"));

		TEST_ASSERT(*arr.Begin() == "insert_start");
		TEST_ASSERT(*(arr.End() - 1) == "insert_end");
	}
	{
		arr.Erase(arr.Begin());
		arr.Erase(arr.End() - 1);
		TEST_ASSERT(arr.GetSize() == 10);
	}
	{
		SIZET idx = 0;
		for (Array<AString>::ConstIter iter = arr.Begin(); iter != arr.End(); ++iter, ++idx)
		{
			TEST_ASSERT(*iter == "iterator");
			TEST_ASSERT(idx == arr.Index(iter));
		}
	}
}

// ArrayAllocator
//------------------------------------------------------------------------------
void TestArray::ArrayAllocator() const
{
	{
		StackArray<int32, 20> stackArray1{ 1, 2, 3 };
		StackArray<int32, 20> stackArray2{ 4, 5, 6 };
		stackArray1 = stackArray2;
		for (SIZET idx = 0; idx < stackArray1.GetSize(); ++idx)
		{
			TEST_ASSERT(idx + 4 == stackArray1[idx]);
		}

		stackArray1.Append(7);
		stackArray1.Append(8);
	}
	{
		StackArray<int32, 2> stackArr{ 1, 2, 3 };
	}
}

//------------------------------------------------------------------------------