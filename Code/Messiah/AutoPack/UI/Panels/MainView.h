// FileBrowser.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_PREVIEW_H
#define MESSIAH_AUTOPACK_PREVIEW_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "AutoPack/UI/Basic/UIWidget.h"

// Class GuiMainView
//------------------------------------------------------------------------------
class UIMainView : public UIWidget
{
public:
	UIMainView();
	~UIMainView();

	virtual void OnFrameUpdate();

	TREFLECTION_DECLARE(UIMainView, UIWidget)
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_PREVIEW_H