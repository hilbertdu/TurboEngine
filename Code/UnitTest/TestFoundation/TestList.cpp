// TestList.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "TestFramework/UnitTest.h"

#include "Foundation/Container/List.h"
#include "Foundation/String/String.h"
#include "Foundation/Memory/Allocator.h"

// TestString
//------------------------------------------------------------------------------
class TestList : public UnitTest
{
private:
	DECLARE_TESTS

	void ListConstructors() const;
	void ListInsertErase() const;
	void ListIterator() const;
	void ListAllocator() const;
};

// Register Tests
//------------------------------------------------------------------------------
REGISTER_TESTS_BEGIN(TestList)
	REGISTER_TEST(ListConstructors)
	REGISTER_TEST(ListInsertErase)
	REGISTER_TEST(ListIterator)
	REGISTER_TEST(ListAllocator)
REGISTER_TESTS_END


// ListConstructors
//------------------------------------------------------------------------------
void TestList::ListConstructors() const
{
	LinkedList<int> list;
	{
		LinkedList<int> list1, list2(1);
		TEST_ASSERT(list1.GetSize() == 0);
		TEST_ASSERT(list2.GetSize() == 1);
	}
	{
		LinkedList<int> list1 = list;
		TEST_ASSERT(list1.GetSize() == 0);
	}
	{
		LinkedList<int> list1;
		list1 = list;
		TEST_ASSERT(list1.GetSize() == 0);
	}
}

// ListInsertErase
//------------------------------------------------------------------------------
#include <list>
void TestList::ListInsertErase() const
{
	LinkedList<AString> list;
	{
		list.Insert(list.Begin(), AString("test1"));
		list.Insert(list.Begin(), AString("test0"));
		TEST_ASSERT(*list.Begin() == "test0");
		TEST_ASSERT(*list.Last() == "test1");
	}
	{
		list.PopFront();
		TEST_ASSERT(*list.Begin() == "test1");
		TEST_ASSERT(list.GetSize() == 1);
	}
	{
		list.Insert(list.Begin(), AString("test0"));
		list.PopBack();
		TEST_ASSERT(*list.Begin() == "test0");
		TEST_ASSERT(list.GetSize() == 1);
	}
	{
		list.PushBack(AString("test1"));
		list.PushBack(AStackString<>("test2"));
		TEST_ASSERT(list.GetSize() == 3);
		TEST_ASSERT(*list.Last() == "test2");
	}
	{
		list.EmplaceFront("emplace0");
		list.EmplaceBack(AString("emplace1"));
		TEST_ASSERT(*list.Begin() == "emplace0");
		TEST_ASSERT(*list.Last() == "emplace1");
	}
	{
		list.Clear();
		TEST_ASSERT(list.GetSize() == 0);
	}
	{
		LinkedList<AString>::Iter iter1 = list.Insert(list.Begin(), 10, AStackString<>("insert0"));
		LinkedList<AString>::Iter iter2 = list.Insert(list.End(), 10, AStackString<>("insert1"));
		TEST_ASSERT(*list.Begin() == "insert0");
		TEST_ASSERT(*list.Last() == "insert1");
		TEST_ASSERT(*iter1 == "insert0");
		TEST_ASSERT(*iter2 == "insert1");
	}
	{
		LinkedList<int64> list;
		Timer t;
		for (int64 idx = 0; idx < 100000; ++idx)
		{
			list.PushBack(idx);
		}
		LOUTPUT("List : %2.4fs\n", t.GetElapsed());
	}
	{
		std::list<int64> list;
		Timer t;
		for (int64 idx = 0; idx < 100000; ++idx)
		{
			list.push_back(idx);
		}
		LOUTPUT("std::list : %2.4fs\n", t.GetElapsed());
	}
}

// ListIterator
//------------------------------------------------------------------------------
void TestList::ListIterator() const
{
	LinkedList<AString> list(10);
	{
		for (LinkedList<AString>::Iter iter = list.Begin(); iter != list.End(); ++iter)
		{
			(*iter) = AString("iterator");
		}
	}
	{
		list.Insert(list.Begin(), AString("insert_start"));
		list.Insert(list.End(), AString("insert_end"));

		TEST_ASSERT(*list.Begin() == "insert_start");
		TEST_ASSERT(*(--list.End()) == "insert_end");
	}
	{
		list.Erase(list.Begin());
		list.Erase(list.Last());
		TEST_ASSERT(list.GetSize() == 10);
	}
	{
		SIZET idx = 0;
		for (LinkedList<AString>::ConstIter iter = list.Begin(); iter != list.End(); ++iter, ++idx)
		{
			TEST_ASSERT(*iter == "iterator");
		}
	}
}

// ListAllocator
//------------------------------------------------------------------------------
void TestList::ListAllocator() const
{
	{
		using AllocatorType = PoolAllocator<TAllocForm<sizeof(ListNode<AString>)>>;
		LinkedList<AString, AllocatorType> list;
		
		for (uint16 idx = 0; idx < 10000; ++idx)
		{
			list.PushBack(AStackString<>("test string"));
		}
	}
}

//------------------------------------------------------------------------------