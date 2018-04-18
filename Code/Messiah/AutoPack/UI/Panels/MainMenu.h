// MainMenu.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_MAINMENU_H
#define MESSIAH_AUTOPACK_MAINMENU_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "AutoPack/UI/Basic/UIWidget.h"

// class MAINMENU
//------------------------------------------------------------------------------
class UIMainMenu : public UIWidget
{
public:
	UIMainMenu() {}

	virtual void OnFrameUpdate();

private:
	bool m_Saved;
	bool m_ServiceAdded;

	TREFLECTION_DECLARE(UIMainMenu, UIWidget)
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_MAINMENU_H