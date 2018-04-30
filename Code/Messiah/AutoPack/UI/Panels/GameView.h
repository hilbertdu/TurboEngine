// FileBrowser.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_PREVIEW_H
#define MESSIAH_AUTOPACK_PREVIEW_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/Pattern/Delegate.h"
#include "AutoPack/UI/Basic/UIWidget.h"
#include <windows.h>


// Forward declaration
//------------------------------------------------------------------------------
class MessiahGame;

// Class UIMainView
//------------------------------------------------------------------------------
class UIGameView : public UIWidget
{
public:
	UIGameView();
	~UIGameView();

	virtual void OnFrameUpdate();

private:
	MessiahGame * m_MGame;

	TREFLECTION_DECLARE(UIGameView, UIWidget)
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_PREVIEW_H