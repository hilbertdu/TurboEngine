// UnitTestManager
//------------------------------------------------------------------------------
#pragma once
#ifndef TESTFRAMEWORK_UNITTESTMANAGER_H
#define TESTFRAMEWORK_UNITTESTMANAGER_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Env/Assert.h"
#include "Foundation/Platform/Platform.h"
#include "Foundation/Platform/Types.h"
#include "Foundation/String/String.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/Time/Timer.h"
#include "Foundation/Memory/Mem.h"

// Forward Declarations
//------------------------------------------------------------------------------
class UnitTest;

// UnitTestManager
//------------------------------------------------------------------------------
class UnitTestManager
{
public:
	UnitTestManager();
	~UnitTestManager();

	// Run all tests, or tests from a group
	bool RunTests(const char * testGroup = nullptr);

	// Singleton behaviour
	static inline UnitTestManager &	Get() { return *s_Instance; }

	static inline bool IsValid() { return (s_Instance != 0); }

	// tests register (using the test declaration macros) via this interface
	static void RegisterTestGroup(UnitTest * testGroup);

	// When tests are being executed, they are wrapped with these
	void TestBegin(UnitTest * testGroup, const char * testName);
	void TestEnd();

	// TEST_ASSERT uses this interface to notify of assertion failures
	static bool AssertFailure(const char * message, const char * file, int32 line);

private:
	Timer m_Timer;

	enum : uint32 { MAX_TESTS = 1024 };
	struct TestInfo
	{
		TestInfo() :m_TestGroup(nullptr), m_TestName(nullptr), m_Passed(false), m_MemoryLeaks(false), m_TimeTaken(0.0f) {}

		UnitTest *      m_TestGroup;
		const char *    m_TestName;
		bool            m_Passed;
		bool            m_MemoryLeaks;
		float           m_TimeTaken;
	};
	static uint32		s_NumTests;
	static TestInfo     s_TestInfos[MAX_TESTS];

	static UnitTestManager * s_Instance;
	static UnitTest * s_FirstTest;
};

//------------------------------------------------------------------------------
#endif // TESTFRAMEWORK_UNITTESTMANAGER_H
