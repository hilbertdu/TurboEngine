// TestMap.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "TestFramework/UnitTest.h"
#include "Foundation/Container/AnyClass.h"
#include "Foundation/Pattern/Delegate.h"


// TestString
//------------------------------------------------------------------------------
class TestAnyClass : public UnitTest
{
private:
	DECLARE_TESTS

	void AnyClassConstructors() const;
	void AnyClassAssignment() const;
	void AnyClassCast() const;
};

// Register Tests
//------------------------------------------------------------------------------
REGISTER_TESTS_BEGIN(TestAnyClass)
	REGISTER_TEST(AnyClassConstructors)
	REGISTER_TEST(AnyClassAssignment)
	REGISTER_TEST(AnyClassCast)
REGISTER_TESTS_END


// AnyClassConstructors
//------------------------------------------------------------------------------
void TestAnyClass::AnyClassConstructors() const
{
	{
		AnyClass anyInt(100);
		AnyClass anyFloat(100.0f);
		AnyClass anyString(AString("abcde"));
		AnyClass anyArray(Array<int>(10));
	}
	{
		AnyClass anyInt1(100);
		AnyClass anyInt2(anyInt1);
		AnyClass anyString1(AString("abcde"));
		AnyClass anyString2(std::move(anyString1));
		TEST_ASSERT(anyString1.IsEmpty());
	}
}

// AnyClassAssignment
//------------------------------------------------------------------------------
void TestAnyClass::AnyClassAssignment() const
{
	AnyClass any;
	{
		any = 100;
		any = "abcde";
		any = Array<int>(10);
	}
}

bool TestCompare(int a, int b) { return a < b; }

// AnyClassCast
//------------------------------------------------------------------------------
void TestAnyClass::AnyClassCast() const
{
	{
		AnyClass any("abced");
		char* const* content = any.Cast<char*>();
		//const char** content = any.Cast<char*>(); // understande the difference of this two declaration
		TEST_ASSERT(*content == "abced");
	}
	{
		AnyClass any(100);
		TEST_ASSERT(*any.Cast<int>() == 100);
	}
	{
		AnyClass any(AString("10000"));
		const AString* str = any.Cast<AString>();
		TEST_ASSERT(*any.Cast<AString>() == "10000");

		AString* str2 = any.Cast<AString>();
		str2->Clear();
		TEST_ASSERT(any.Cast<AString>()->IsEmpty());
	}
	{
		AnyDelegate anyDelegate;
		anyDelegate = &TestCompare;
		TEST_ASSERT(anyDelegate.Cast<Delegate<bool(int, int)>>()->Invoke(100, 200));
	}
}

//------------------------------------------------------------------------------