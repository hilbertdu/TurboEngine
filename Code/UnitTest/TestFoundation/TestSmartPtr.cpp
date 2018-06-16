// TestSmartPtr.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "TestFramework/UnitTest.h"

#include "Foundation/Platform/Types.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/Container/SmartPtr.h"
#include "Foundation/Objects/RefObject.h"


// TestMemPoolBlock
//------------------------------------------------------------------------------
class TestSmartPtr : public UnitTest
{
private:
	DECLARE_TESTS

	void TestStrongPtr() const;
	void TestWeakPtr() const;
	void TestDeletor() const;
	void TestRefProxy() const;
};

// Register Tests
//------------------------------------------------------------------------------
REGISTER_TESTS_BEGIN(TestSmartPtr)
	REGISTER_TEST(TestStrongPtr)
	REGISTER_TEST(TestWeakPtr)
	REGISTER_TEST(TestDeletor)
	REGISTER_TEST(TestRefProxy)
REGISTER_TESTS_END

// TestStrongPtr
//------------------------------------------------------------------------------
void TestSmartPtr::TestStrongPtr() const
{
	{
		StrongPtr<char> ptr(TNEW(char[8]{ 0 }));
		ptr.Get()[0] = '1';
		TEST_ASSERT(AString(ptr.Get()) == "1");
	}
	{
		StrongPtr<AString> ptr1(TNEW(AString("test")));
		StrongPtr<AString> ptr2 = ptr1;

		TEST_ASSERT(ptr1.Get() == ptr2.Get());

		ptr1.Set(ptr1.Get());	// test self set
		TEST_ASSERT(ptr1.Get() == ptr2.Get());

		ptr1.Set(nullptr);		// test reset
		TEST_ASSERT(ptr1.Get() == nullptr);
		TEST_ASSERT(*ptr2.Get() == "test");
	}
	{
		StrongPtr<int> ptr1(TNEW(int(2)));
		StrongPtr<int> ptr2;

		ptr2 = ptr1;
		TEST_ASSERT(ptr1.Get() == ptr2.Get());
		TEST_ASSERT(*ptr1.Get() == 2);
		TEST_ASSERT(*ptr2.Get() == 2);
	}
}

// TestWeakPtr
//------------------------------------------------------------------------------
void TestSmartPtr::TestWeakPtr() const
{
	int * p = TNEW(int(2));
	StrongPtr<int> ptr(p);
	WeakPtr<int> wptr;

	{
		WeakPtr<int> wptr1(ptr);
		TEST_ASSERT(*wptr1.Get() == 2);
	}
	{
		wptr = ptr;
		TEST_ASSERT(*wptr.Get() == 2);

		ptr.Set(nullptr);
		TEST_ASSERT(wptr.Expired());
	}
	{
		ptr = StrongPtr<int>(TNEW(int(3)));
		TEST_ASSERT(wptr.Expired());
		wptr = ptr;
		TEST_ASSERT(*wptr.Get() == 3);

		*ptr.Get() = 4;
		TEST_ASSERT(*wptr.Get() == 4);
	}
}

// TestDeletor
//------------------------------------------------------------------------------
void TestSmartPtr::TestDeletor() const
{
	{
		StrongPtr<int> ptr(TNEW(int(2)));
	}
	{
		StrongPtr<char, ArrayDeletor<char>> ptr(TNEW(char[16]));
	}
	{
		StrongPtr<char, AllocDeletor> ptr((char*)ALLOC(100));
	}
	{
		PoolAllocator<TAllocForm<32, 4>> poolAllocator;
		StrongPtr<char, PoolDeletor<TAllocForm<32, 4>>> ptr1((char*)poolAllocator.Allocate(30), poolAllocator.GetDeletor());

		auto& deletor = poolAllocator.GetDeletor();
		StrongPtr<char, PoolDeletor<TAllocForm<32, 4>>> ptr2((char*)poolAllocator.Allocate(30), deletor);
	}
}


// TestRefProxy
//------------------------------------------------------------------------------
class RefA : public RefObject<RefA>
{
public:
	int m_Int = 100;
};

void TestSmartPtr::TestRefProxy() const
{
	{
		RefA * a = new RefA();
		RefProxy<RefA> a2 = a->GetRefProxy();
		TEST_ASSERT(a2.Get() == a);
		delete a;
		TEST_ASSERT(a2.Get() == nullptr);
	}

	RefProxy<RefA> proxy_a;
	{
		RefA a;
		proxy_a = a.GetRefProxy();
		TEST_ASSERT(proxy_a.Get() == &a);
	}
	TEST_ASSERT(proxy_a.Get() == nullptr);
}

//------------------------------------------------------------------------------