// FileBrowser.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_FILE_PROPERTY_H
#define MESSIAH_AUTOPACK_FILE_PROPERTY_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "AutoPack/UI/Basic/UIWidget.h"

// Class GuiMainView
//------------------------------------------------------------------------------
class UIFileProperty : public UIWidget
{
public:
	UIFileProperty();
	~UIFileProperty();

	virtual void OnFrameUpdate();

	TREFLECTION_DECLARE(UIFileProperty, UIWidget)
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_FILE_PROPERTY_H