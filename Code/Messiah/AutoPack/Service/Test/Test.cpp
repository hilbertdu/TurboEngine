// Test.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Test.h"
#include "../Command.h"

// Constructor
//------------------------------------------------------------------------------
Test::Test() 
	: IService("Test")
{
	m_Commands.Append(TNEW(ExeCommand("TestBat")));
}

// Destructor
//------------------------------------------------------------------------------
Test::~Test() 
{}

// Initialize
//------------------------------------------------------------------------------
/*virtual*/ void Test::Initialize()
{
	IParameter* cmdName = TNEW(IParameter("D:\\Projects\\GitProjects\\TurboEngine\\Test\\Test.bat"));
	m_Commands[0]->SetParameterLen(1);
	m_Commands[0]->SetParameter(0, cmdName);
}

// Finalize
//------------------------------------------------------------------------------
/*virtual*/ void Test::Finalize() 
{}

//------------------------------------------------------------------------------