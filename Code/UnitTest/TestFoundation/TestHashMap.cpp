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
	void HashMapGrow() const;
	void HashMapAllocator() const;
	void HashMapMove() const;
};

// Register Tests
//------------------------------------------------------------------------------
REGISTER_TESTS_BEGIN(TestHashMap)
	REGISTER_TEST(HashMapConstructors)
	REGISTER_TEST(HashMapAssignment)
	REGISTER_TEST(HashMapIterator)
	REGISTER_TEST(HashMapGrow)
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
		pair = { 2, AString("22") };
	}
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
		for (HashMap<int, int>::Iter iter = map.Begin(); iter != map.End(); ++iter)
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
		for (HashMap<int, AString>::ConstIter iter = map.Begin(); iter != map.End(); ++iter)
		{
			TEST_ASSERT((*iter).Second() == "test");
		}
		HashMap<int, AString>::Iter iter1 = map.Find(1);
		HashMap<int, AString>::ConstIter iter2 = map.Find(1);
		AString s1 = (*iter1).Second();
		const AString s2 = (*iter2).Second();
	}
}

// HashMapGrow
//------------------------------------------------------------------------------
#include <unordered_map>
void TestHashMap::HashMapGrow() const
{
	{
		HashMap<int64, AString> map;
		Timer t;
		int64 maxSize = 10000;
		for (int64 index = 0; index < maxSize; ++index)
		{
			map[index] = "test";// AString("test");
		}
		LOUTPUT("HashMap string : %2.4fs\n", t.GetElapsed());
		TEST_ASSERT(map[10] == "test");
	}
	{
		std::unordered_map<int64, AString> stdMap;
		Timer t;
		int64 maxSize = 10000;
		for (int64 index = 0; index < maxSize; ++index)
		{
			stdMap[index] = "test";// AString("test");
		}
		LOUTPUT("std::unordered_map string : %2.4fs\n", t.GetElapsed());
	}
	{
		HashMap<int64, int64> map;
		Timer t;
		int64 maxSize = 10000;
		for (int64 index = 0; index < maxSize; ++index)
		{
			map[index] = index;
		}
		LOUTPUT("HashMap int : %2.4fs\n", t.GetElapsed());
		TEST_ASSERT(map[10] == 10);
	}
	{
		std::unordered_map<int64, int64> stdMap;
		Timer t;
		int64 maxSize = 10000;
		for (int64 index = 0; index < maxSize; ++index)
		{
			stdMap[index] = index;
		}
		LOUTPUT("std::unordered_map int : %2.4fs\n", t.GetElapsed());
	}
}

// HashMapAllocator
//------------------------------------------------------------------------------
void TestHashMap::HashMapAllocator() const
{
	{
		HashMap<int, int*> map;
		map[1] = new int(1);
		map[2] = new int(2);
		map[3] = new int(1);
		map[4] = new int(2);
		map[5] = new int(1);
		map[6] = new int(2);
		map[7] = new int(1);
		map[8] = new int(2);

		for (HashMap<int, int*>::Iter iter = map.Begin(); iter != map.End(); ++iter)
		{
			TDELETE_SAFE((*iter).Second());
		}
		map.Clear();
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