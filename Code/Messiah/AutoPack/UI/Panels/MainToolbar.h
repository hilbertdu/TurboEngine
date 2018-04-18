// MainToolBar.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_MAINTOOLBAR_H
#define MESSIAH_AUTOPACK_MAINTOOLBAR_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "../Basic/UIWidget.h"
#include "../UISystem.h"

// class MAINToolBar
//------------------------------------------------------------------------------
class UIMainToolBar : public UIWidget
{
public:
	UIMainToolBar() {}

	virtual void OnFrameUpdate();

	TREFLECTION_DECLARE(UIMainToolBar, UIWidget)
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_MAINTOOLBAR_H