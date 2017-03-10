// TestEvent.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "TestFramework/UnitTest.h"

#include "Foundation/Platform/Types.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/Pattern/Event.h"


// TestMemPoolBlock
//------------------------------------------------------------------------------
class TestDelegateEvent : public UnitTest
{
private:
	DECLARE_TESTS

	void TestDelegate() const;
	void TestEvent() const;
	void TestRemoveOwner() const;
};

// Register Tests
//------------------------------------------------------------------------------
REGISTER_TESTS_BEGIN(TestDelegateEvent)
	REGISTER_TEST(TestDelegate)
	REGISTER_TEST(TestEvent)
	REGISTER_TEST(TestRemoveOwner)
REGISTER_TESTS_END

// Test function
//------------------------------------------------------------------------------
void Test1_0(int a)
{
	LOUTPUT("Test1_0: %d\n", a);
}

void Test1_1(int a)
{
	LOUTPUT("Test1_1: %d\n", a);
}

void Test2(int a, float b)
{
	LOUTPUT("Test2: %d, %.2f\n", a, b);
}

// Test bound method
//------------------------------------------------------------------------------
class TestA
{
public:
	void Test1(int a)
	{
		LOUTPUT("TestA1: %d\n", a);
	}

	void Test2(int a, float b)
	{
		LOUTPUT("TestA2: %d, %.2f\n", a, b);
	}
};

// Test Event
//------------------------------------------------------------------------------
class TestEvent
{
public:
	//typedef Signature<void(int, float)>::Event EventType;

	void Trigger(int a, float b)
	{
		LOUTPUT("TestEvent trigger\n");
		//event.Signal(a, b);
	}

	//EventType event;
};

TestEvent* g_Event = new TestEvent();

class TestListener
{
public:
	void OnTrigger1(int a, float b)
	{
		LOUTPUT("TestEvent on trigger1: %d, %.2f\n", a, b);
		// remove event
		if (g_Event)
		{
			LOUTPUT("Delete global event in event ontrigger function");
			delete g_Event;
			g_Event = nullptr;
		}
	}

	void OnTrigger2(int a, float b)
	{
		LOUTPUT("TestEvent on trigger2: %d, %.2f\n", a, b);
		Remove();
	}

	void Listen()
	{
	}

	void Remove()
	{
	}

	~TestListener()
	{
		Remove();
	}
};

// TestStrongPtr
//------------------------------------------------------------------------------
void TestDelegateEvent::TestDelegate() const
{
	{
		Signature<void(int)>::Delegate delegate;
		delegate.BindFunction(&Test1_0);
		delegate.Invoke(10);
		delegate.Unbind();
		TEST_ASSERT(!delegate.IsValid());
		LOUTPUT("Fast delegate size: %d\n", sizeof(delegate));
	}
	{
		TestA a;
		Signature<void(int)>::Delegate delegate;
		delegate.BindMethod(&a, &TestA::Test1);
		delegate.Invoke(20);
		delegate.Unbind();
		TEST_ASSERT(!delegate.IsValid());
	}
	{
		Signature<void(int)>::Delegate delegate;
		delegate.BindLambda([](int a) {
			LOUTPUT("Test lambda: %d\n", a);
		});
		delegate.Invoke(30);
		delegate.Unbind();
		TEST_ASSERT(!delegate.IsValid());
	}
	{
		int64 a = 1;
		int64 b = 1;
		int64 c = 1;
		int64 d = 1;
		Signature<void(int)>::Delegate delegate;
		delegate.BindLambda([&a, &b, &c, &d](int x) {
			LOUTPUT("Test lambda: %d (closure: %lld, %lld, %lld, %lld)\n", x, a, b, c, d);
		});
		delegate.Invoke(30);
		delegate.Unbind();
		TEST_ASSERT(!delegate.IsValid());
	}
}

// TestWeakPtr
//------------------------------------------------------------------------------
void TestDelegateEvent::TestEvent() const
{
}

// TestRemoveOwner
//------------------------------------------------------------------------------
void TestDelegateEvent::TestRemoveOwner() const
{
}

//------------------------------------------------------------------------------