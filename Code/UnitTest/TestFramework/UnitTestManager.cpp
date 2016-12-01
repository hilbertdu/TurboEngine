// UnitTestManager.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "UnitTestManager.h"
#include "UnitTest.h"

#include <string.h>
#if defined(__WINDOWS__)
#include <windows.h>
#endif

// Static Data
//------------------------------------------------------------------------------
/*static*/ uint32 UnitTestManager::s_NumTests(0);
/*static*/ UnitTestManager::TestInfo UnitTestManager::s_TestInfos[MAX_TESTS];
/*static*/ UnitTestManager * UnitTestManager::s_Instance = nullptr;
/*static*/ UnitTest * UnitTestManager::s_FirstTest = nullptr;

// CONSTRUCTOR
//------------------------------------------------------------------------------
UnitTestManager::UnitTestManager()
{
	// Manage singleton
	ASSERT(s_Instance == nullptr);
	s_Instance = this;

	// If we're running outside the debugger, we don't want
	// failures to pop up a dialog.  We want them to throw so
	// the test framework can catch the exception
#ifdef ASSERT_ENABLED
	if (IsDebuggerAttached() == false)
	{
		AssertHandler::SetThrowOnAssert(true);
	}
#endif
}

// DESTRUCTOR
//------------------------------------------------------------------------------
UnitTestManager::~UnitTestManager()
{
#ifdef ASSERT_ENABLED
	if (IsDebuggerAttached() == false)
	{
		AssertHandler::SetThrowOnAssert(false);
	}
#endif

	// free all registered tests
	UnitTest * testGroup = s_FirstTest;
	while (testGroup)
	{
		UnitTest * next = testGroup->m_NextTestGroup;
		TDELETE testGroup;
		testGroup = next;
	}

	// manage singleton
	ASSERT(s_Instance == this);
	s_Instance = nullptr;
}

// RegisterTest
//------------------------------------------------------------------------------
/*static*/ void UnitTestManager::RegisterTestGroup(UnitTest * testGroup)
{
	// first ever test? place as head of list
	if (s_FirstTest == nullptr)
	{
		s_FirstTest = testGroup;
		return;
	}

	// link to end of list
	UnitTest * thisGroup = s_FirstTest;
loop:
	if (thisGroup->m_NextTestGroup == nullptr)
	{
		thisGroup->m_NextTestGroup = testGroup;
		return;
	}
	thisGroup = thisGroup->m_NextTestGroup;
	goto loop;
}

// RunTests
//------------------------------------------------------------------------------
bool UnitTestManager::RunTests(const char * testGroup)
{
	// check for compile time filter
	UnitTest * test = s_FirstTest;
	while (test)
	{
		if (testGroup != nullptr)
		{
			// is this test the one we want?
			if (AString::StrNCmp(test->GetName(), testGroup, strlen(testGroup)) != 0)
			{
				// no -skip it
				test = test->m_NextTestGroup;
				continue;
			}
		}

		LOUTPUT("------------------------------\n");
		const char * testName = test->GetName();
		LOUTPUT("Test Group: %s\n", test->GetName());
		try
		{
			test->RunTests();
		}
		catch (...)
		{
			LOUTPUT(" - Test '%s' *** FAILED ***\n", s_TestInfos[s_NumTests - 1].m_TestName);
		}
		test = test->m_NextTestGroup;
	}

	LOUTPUT("------------------------------------------------------------\n");
	LOUTPUT("Summary For All Tests\n");

	int32 numPassed = 0;
	UnitTest * lastGroup = nullptr;
	for (size_t i = 0; i < s_NumTests; ++i)
	{
		const TestInfo& info = s_TestInfos[i];
		if (info.m_TestGroup != lastGroup)
		{
			LOUTPUT("------------------------------------------------------------\n");
			LOUTPUT("                :        : %s\n", info.m_TestGroup->GetName());
			lastGroup = info.m_TestGroup;
		}

		const char * status = "OK";
		if (info.m_Passed)
		{
			++numPassed;
		}
		else
		{
			status = (info.m_MemoryLeaks) ? "FAILED (LEAKS)" : "FAILED";
		}

		LOUTPUT("%15s : %5.1fs : %30s\n", status, info.m_TimeTaken, info.m_TestName);
	}
	LOUTPUT("------------------------------------------------------------\n");
	LOUTPUT(" - Passed: %u / %u (%u failures)\n", numPassed, s_NumTests, (s_NumTests - numPassed));
	LOUTPUT("------------------------------------------------------------\n");

	return (s_NumTests == numPassed);
}

// TestBegin
//------------------------------------------------------------------------------
void UnitTestManager::TestBegin(UnitTest * testGroup, const char * testName)
{
	// record info for this test
	TestInfo& info = s_TestInfos[s_NumTests];
	info.m_TestGroup = testGroup;
	info.m_TestName = testName;
	++s_NumTests;

	LOUTPUT(" - Test '%s'\n", testName);
#if (T_MEM_TRACKER_ENABLE)
	MemTracker::Reset();
#endif
	m_Timer.Start();

#ifdef PROFILING_ENABLED
	ProfileManager::Start(testName);
#endif
}

// TestEnd
//------------------------------------------------------------------------------
void UnitTestManager::TestEnd()
{
#ifdef PROFILING_ENABLED
	ProfileManager::Stop();

	// flush the profiling buffers, but don't profile the sync itself
	// (because it leaves an outstanding memory alloc, it looks like a leak)
	ProfileManager::SynchronizeNoTag();
#endif

	float timeTaken = m_Timer.GetElapsed();

	TestInfo& info = s_TestInfos[s_NumTests - 1];
	info.m_TimeTaken = timeTaken;

#ifdef MEMTRACKER_ENABLED
	if (MemTracker::GetCurrentAllocationCount() != 0)
	{
		info.m_MemoryLeaks = true;
		OUTPUT(" - Test '%s' in %2.3fs : *** FAILED (Memory Leaks)***\n", info.m_TestName, timeTaken);
		MemTracker::DumpAllocations();
		TEST_ASSERT(false);
		return;
	}
#endif

	LOUTPUT(" - Test '%s' in %2.3fs : PASSED\n", info.m_TestName, timeTaken);
	info.m_Passed = true;
}

// Assert
//------------------------------------------------------------------------------
/*static*/ bool UnitTestManager::AssertFailure(const char * message, const char * file, int32 line)
{
	LOUTPUT("\n-------- TEST ASSERTION FAILED --------\n");
	LOUTPUT("%s(%i): Assert: %s", file, line, message);
	LOUTPUT("\n-----^^^ TEST ASSERTION FAILED ^^^-----\n");

	if (IsDebuggerAttached())
	{
		return true; // tell the calling code to break at the failure sight
	}

	// throw will be caught by the unit test framework and noted as a failure
	throw "Test Failed";
}

//------------------------------------------------------------------------------
