// ApplicationWin.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Application.h"


/*static*/ void Application::Initialize()
{
	Singleton<Application>::Initialize<ApplicationWin32>();
}

/*static*/ void Application::Finalize()
{
	Singleton<Application>::Finalize();
}