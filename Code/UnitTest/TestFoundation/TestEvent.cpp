// TestEvent.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "TestFramework/UnitTest.h"

#include "Foundation/Platform/Types.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/Pattern/Event.h"

#include <functional>


// TestMemPoolBlock
//------------------------------------------------------------------------------
class TestDelegateEvent : public UnitTest
{
private:
	DECLARE_TESTS

	void TestDelegate() const;
	void TestDelegatePerformance() const;
	void TestEventSignal() const;
	void TestRemoveOwner() const;
	void TestAnyDelegate() const;
};

// Register Tests
//------------------------------------------------------------------------------
REGISTER_TESTS_BEGIN(TestDelegateEvent)
	REGISTER_TEST(TestDelegate)
	REGISTER_TEST(TestDelegatePerformance)
	REGISTER_TEST(TestEventSignal)
	REGISTER_TEST(TestRemoveOwner)
	REGISTER_TEST(TestAnyDelegate)
REGISTER_TESTS_END

// Test function
//------------------------------------------------------------------------------
void Test1(int a)
{
	int32 b = a * 100;
	int32 c = b * 20 + 2017;
	//LOUTPUT("Test1_0: %d\n", a);
}

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
		LOUTPUT("TestA1: %d, %d\n", a, m_Member);
	}

	void Test2(int a, float b)
	{
		LOUTPUT("TestA2: %d, %.2f, %d\n", a, b, m_Member);
	}

	void Test3(const char * str)
	{
		LOUTPUT("TestA3: %s, %d\n", str, m_Member);
	}

private:
	int m_Member{ 200 };
};

// Test Event
//------------------------------------------------------------------------------
class TestEvent
{
public:
	typedef Signature<void(int, float)>::Event EventType;

	void Trigger(int a, float b)
	{
		LOUTPUT("TestEvent trigger\n");
		event.Signal(a, b);
	}

	EventType event;
};

TestEvent* g_Event = nullptr;

class TestListener
{
public:
	void OnTrigger1(int a, float b)
	{
		LOUTPUT("TestEvent on trigger1: %d, %.2f, %d\n", a, b, m_Member);
		// remove event
		if (g_Event)
		{
			LOUTPUT("Delete global event in event ontrigger function\n");
			delete g_Event;
			g_Event = nullptr;
		}
	}

	void OnTrigger2(int a, float b)
	{
		LOUTPUT("TestEvent on trigger2: %d, %.2f, %d\n", a, b, m_Member);
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

private:
	int m_Member{ 100 };
};

// TestDelegate
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
		LOUTPUT("std function size: %d\n", sizeof(std::function<void(int)>));		
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
		TestA a;
		Signature<void(const char*)>::Delegate delegate;
		delegate.BindMethod(&a, &TestA::Test3);
		delegate.Invoke("abcdef");
		delegate.Unbind();
		TEST_ASSERT(!delegate.IsValid());
	}
	{
		Signature<void(int)>::Delegate delegate([](int a) {
		});
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
	{
		TestA a;
		Delegate<void(int)> d;
		d.BindMethod<TestA>(nullptr, &TestA::Test1);
		d.Invoke(&a, 100);
	}
}

// TestDelegatePerformance
//------------------------------------------------------------------------------
void TestDelegateEvent::TestDelegatePerformance() const
{
	{
		std::function<void(int)> func1 = &Test1;
		Signature<void(int)>::Delegate delegate1;
		delegate1.BindFunction(&Test1);

		float time1(0.0f);
		float time2(0.0f);
		float time3(0.0f);

		Timer t1;
		for (uint32 idx = 0; idx < 1000000; ++idx)
		{
			func1(idx);
		}
		time1 = t1.GetElapsed();

		Timer t2;
		for (uint32 idx = 0; idx < 1000000; ++idx)
		{
			delegate1.Invoke(idx);
		}
		time2 = t2.GetElapsed();

		Timer t3;
		for (uint32 idx = 0; idx < 1000000; ++idx)
		{
			Test1(idx);
		}
		time3 = t3.GetElapsed();

		LOUTPUT("std::function : %2.4fs\n", time1);
		LOUTPUT("fast delegate : %2.4fs\n", time2);
		LOUTPUT("raw function  : %2.4fs\n", time3);
	}
}

// TestWeakPtr
//------------------------------------------------------------------------------
void TestDelegateEvent::TestEventSignal() const
{
	{
		Signature<void(int)>::Event event;
		int64 id1 = event.Add(&Test1_0);
		int64 id2 = event.Add(&Test1_1);

		int64 id3 = event.Add([](int a) {
			LOUTPUT("Test event lambda: %d\n", a);
		});

		TestA a;
		int64 id4 = event.Add(Signature<void(int)>::Delegate(&a, &TestA::Test1));

		event.Signal(100);
		event.Remove(id1);
		event.Signal(200);
		event.Remove(id2);
		event.Signal(300);
		event.Remove(id3);
		event.Signal(400);
	}
	{
		Signature<void(const char*)>::Event event;
		TestA a;
		event.Add(Signature<void(const char*)>::Delegate(&a, &TestA::Test3));

		const char * str = "abcedf-----gh";
		event.Signal(str);
	}
}

// TestRemoveOwner
//------------------------------------------------------------------------------
void TestDelegateEvent::TestRemoveOwner() const
{
	g_Event = new TestEvent();
	using Delegate1 = Signature<void(int, float)>::Delegate;
	{
		TestListener listener1;
		TestA a;
		g_Event->event.Add(Delegate1(&listener1, &TestListener::OnTrigger1));
		g_Event->event.Add(Delegate1(&a, &TestA::Test2));

		g_Event->event.Signal(10, 100);
		TEST_ASSERT(g_Event == nullptr);
	}
}

// TestAnyDelegate
//------------------------------------------------------------------------------
void TestDelegateEvent::TestAnyDelegate() const
{
	AnyDelegate anyDelegate(&Test1_0);
	AnyDelegate anyDelegate0((TestA*)0, &TestA::Test1);
	{
		anyDelegate = Delegate<void(int)>(&Test1_0);
		anyDelegate.Cast<Delegate<void(int)>>()->Invoke(1000);
	}
	{
		TestA a;
		anyDelegate = Delegate<void(int)>(&a, &TestA::Test1);
		anyDelegate.Cast<Delegate<void(int)>>()->Invoke(100);
	}
	{
		anyDelegate = Delegate<void(int)>((TestA*)0, &TestA::Test1);
		TestA a;
		anyDelegate.Cast<Delegate<void(int)>>()->Invoke(&a, 1000);
	}
	{
		anyDelegate = Delegate<void(int, float)>(Test2);
		anyDelegate.Cast<Delegate<void(int, float)>>()->Invoke(1000, 100.0);
	}
}

//------------------------------------------------------------------------------