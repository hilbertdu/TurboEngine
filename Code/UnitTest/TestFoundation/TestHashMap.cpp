// TestMap.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "TestFramework/UnitTest.h"

#include "Foundation/Container/HashMap.h"

// TestString
//------------------------------------------------------------------------------
class TestHashMap : public UnitTest
{
private:
	DECLARE_TESTS

	void HashMapConstructors() const;
	void HashMapAssignment() const;
	void HashMapIterator() const;
	void HashMapAllocator() const;
	void HashMapMove() const;
};

// Register Tests
//------------------------------------------------------------------------------
REGISTER_TESTS_BEGIN(TestHashMap)
	REGISTER_TEST(HashMapConstructors)
	REGISTER_TEST(HashMapAssignment)
	REGISTER_TEST(HashMapIterator)
	REGISTER_TEST(HashMapAllocator)
	REGISTER_TEST(HashMapMove)
REGISTER_TESTS_END


// HashMapConstructors
//------------------------------------------------------------------------------
void TestHashMap::HashMapConstructors() const
{
	Pair<int, AString> pair = { 1, AString("") };
	HashMap<int, AString> map = { pair };
	{
		HashMap<int, AString> map1(map);
		TEST_ASSERT(map1[1] == "");
	}
	{
		HashMap<int, AString> map2 = map;
		TEST_ASSERT(map2[1] == "");
	}
	{
		HashMap<int, AString> map3(17);
		map3 = map;
		TEST_ASSERT(map3[1] == "");
	}
	{
		HashMap<int, AString> map4 = {
			{ 1, AString("test1") },
			{ 2, AString("test2") },
		};
		TEST_ASSERT(map4[1] == "test1");
		TEST_ASSERT(map4[2] == "test2");
	}
}

// HashMapAssignment
//------------------------------------------------------------------------------
void TestHashMap::HashMapAssignment() const
{
	{
		HashMap<int, AString> map1;
		map1[1] = "test1";
		map1[2] = "test2";
		AString m1 = map1[1];
		TEST_ASSERT(map1[1] == "test1");
		TEST_ASSERT(map1[2] == "test2");
		TEST_ASSERT(m1 == "test1");
	}
	{
		HashMap<int, int> map(11);
		for (int idx = 0; idx < 12; ++idx)
		{
			map[idx] = idx + 1;
		}
	}
	{
		HashMap<int, int*> map;
		map[1] = new int(1);
		delete map[1];
	}
}

// HashMapIterator
//------------------------------------------------------------------------------
void TestHashMap::HashMapIterator() const
{
	{
		HashMap<int, int> map;
		map[1] = 1;
		map[2] = 2;
		for (HashMap<int, int>::Iterator iter = map.Begin(); iter != map.End(); ++iter)
		{
			TEST_ASSERT((*iter).First() == (*iter).Second());
			(*iter).Second() = 10;
			TEST_ASSERT((*iter).Second() == 10);
		}
		TEST_ASSERT(map[1] == 10);
		TEST_ASSERT(map[2] == 10);
	}
	{
		HashMap<int, AString> map = { {1, AString("test")} };
		for (HashMap<int, AString>::ConstIterator iter = map.Begin(); iter != map.End(); ++iter)
		{
			TEST_ASSERT((*iter).Second() == "test");
		}
		HashMap<int, AString>::Iterator iter1 = map.Find(1);
		HashMap<int, AString>::ConstIterator iter2 = map.Find(1);
		AString s1 = (*iter1).Second();
		const AString s2 = (*iter2).Second();
	}
}

// HashMapAllocator
//------------------------------------------------------------------------------
void TestHashMap::HashMapAllocator() const
{
	{
	}
}

// HashMapMove
//------------------------------------------------------------------------------
void TestHashMap::HashMapMove() const
{
	{
	}
}

//------------------------------------------------------------------------------