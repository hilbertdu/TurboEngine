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
};

// Register Tests
//------------------------------------------------------------------------------
REGISTER_TESTS_BEGIN(TestDelegateEvent)
	REGISTER_TEST(TestDelegate)
	REGISTER_TEST(TestEvent)
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
	TestA() : event(nullptr) {}
	void TestA2(int a, float b) { 
		LOUTPUT("TestA2: %d, %.2f\n", a, b); 
		if (event)
		{
			event->Unbind<TestA, &TestA::TestA2>(this);
		}
	}

	Signature<void(int, float)>::Event* event;
};

// TestStrongPtr
//------------------------------------------------------------------------------
void TestDelegateEvent::TestDelegate() const
{
	{
		Signature<void(int)>::Delegate delegate;
		delegate.Bind<&Test1_0>();
		delegate.Invoke(1);
		delegate.Invoke(2);
		delegate.Bind<&Test1_1>();
		delegate.Invoke(20);
		delegate.Unbind();
	}
	{
		TestA a;
		Signature<void(int, float)>::Delegate delegate;
		delegate.Bind<&Test2>();
		delegate.Bind<TestA, &TestA::TestA2>(&a);
		delegate.Invoke(1, 2);
		delegate.Unbind();
	}
}

// TestWeakPtr
//------------------------------------------------------------------------------
void TestDelegateEvent::TestEvent() const
{
	{
		Signature<void(int)>::Event event;
		event.Bind<&Test1_0>();
		event.Bind<&Test1_1>();
		event.Signal(10);
		event.Signal(100);
		event.Unbind<&Test1_0>();
		event.Signal(1000);
	}
	{
		TestA a;
		Signature<void(int, float)>::Event event;
		a.event = &event;
		event.Bind<&Test2>();
		event.Bind<TestA, &TestA::TestA2>(&a);
		event.Signal(20, 7.5);

		//event.Unbind<TestA, &TestA::TestA2>(&a);
		event.Signal(30, 8.5);

		event.Unbind<&Test2>();
		event.Unbind<TestA, &TestA::TestA2>(&a);
	}
	{
		TestA a;
		Signature<void(int, float)>::Event event;
		size_t idx1 = event.Bind<&Test2>();
		size_t idx2 = event.Bind<TestA, &TestA::TestA2>(&a);

		event.Unbind<&Test2>();
		event.Signal(40, 10.5);
		event.Unbind(idx2);
		event.Signal(40, 12.5);
	}
}

//------------------------------------------------------------------------------