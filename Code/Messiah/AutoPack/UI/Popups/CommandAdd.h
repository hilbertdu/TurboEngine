// CommandAdd.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_PROJECT_ADD_H
#define MESSIAH_AUTOPACK_PROJECT_ADD_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "AutoPack/UI/Basic/UIWidget.h"

// Class GuiMainView
//------------------------------------------------------------------------------
class UICommandAdd : public UIWidget
{
public:
	UICommandAdd();
	~UICommandAdd();

	virtual void OnFrameUpdate();

private:
	AStackString<32> m_ServiceName;
	AStackString<32> m_CommandName;

	TREFLECTION_DECLARE(UICommandAdd, UIWidget)
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_PROJECT_ADD_H