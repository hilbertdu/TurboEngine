// Main.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Application.h"
#include "../Engine/EngineCore.h"


int main(int, char**)
{
	EngineCore::Initialize();
	Application::Initialize();

	Application::Instance().Run();
	EngineCore::Instance().Tick();

	Application::Finalize();
	EngineCore::Finalize();
	return 0;
}
