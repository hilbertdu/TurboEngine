// ReflectionDecl.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "ReflectionDecl.h"

// BindReflection
//------------------------------------------------------------------------------
void BindReflectionAll()
{
	BIND_REFLECTION(ImGUIDocker);
	BIND_REFLECTION(UIWidget);
	BIND_REFLECTION(UIMainMenu);
	BIND_REFLECTION(UIMainToolBar);
	BIND_REFLECTION(UIAssetBrowser);
	BIND_REFLECTION(UIFileProperty);
	BIND_REFLECTION(UICommands);
	BIND_REFLECTION(UIGameView);
	BIND_REFLECTION(UIOutputPanel);
	BIND_REFLECTION(IService);
	BIND_REFLECTION(IBatchService);
	BIND_REFLECTION(ICommand);
}

//------------------------------------------------------------------------------