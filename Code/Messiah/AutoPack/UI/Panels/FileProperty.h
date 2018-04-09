// FileBrowser.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_FILE_PROPERTY_H
#define MESSIAH_AUTOPACK_FILE_PROPERTY_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "../Basic/UIWidget.h"

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


UIFileProperty::UIFileProperty()
{
	m_Name = "Property";
	m_Parent = "Assets";
}

UIFileProperty::~UIFileProperty()
{}

/*virtual*/ void UIFileProperty::OnFrameUpdate()
{
	if (ImGui::BeginDock(m_Name.Get()))
	{
	}
	ImGui::EndDock();
}


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_FILE_PROPERTY_H