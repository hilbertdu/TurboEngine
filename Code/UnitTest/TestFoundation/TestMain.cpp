// TestMain.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "TestFramework/UnitTest.h"

// main
//------------------------------------------------------------------------------
int main(int, char *[])
{
	// Tests to run
	//REGISTER_TESTGROUP(TestArray)
	//REGISTER_TESTGROUP(TestString)
	//REGISTER_TESTGROUP(TestHashMap)	
	//REGISTER_TESTGROUP(TestSmartPtr)
	//REGISTER_TESTGROUP(TestDelegateEvent)
	//REGISTER_TESTGROUP(TestAnyClass)
	//REGISTER_TESTGROUP(TestMem)
	REGISTER_TESTGROUP(TestReflection)

	UnitTestManager utm;

	bool allPassed = utm.RunTests();

	return allPassed ? 0 : -1;
}
//------------------------------------------------------------------------------
