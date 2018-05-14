// MainMenu.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_MAINMENU_H
#define MESSIAH_AUTOPACK_MAINMENU_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/Container/HashMap.h"
#include "AutoPack/UI/Basic/UIWidget.h"

// class MAINMENU
//------------------------------------------------------------------------------
class UIMainMenu : public UIWidget
{
public:
	UIMainMenu();

	virtual void OnFrameUpdate();

private:
	void DoPreActions();
	void DoPostActions();

	void ActionSave();
	void ActionSaveAs();
	void ActionLoad();
	void ActionRecent();
	void ActionSettings();
	void ActionExit();

	HashMap<AString, bool> m_ViewState;

	TREFLECTION_DECLARE(UIMainMenu, UIWidget)
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_MAINMENU_H