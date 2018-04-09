// FileBrowser.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_PREVIEW_H
#define MESSIAH_AUTOPACK_PREVIEW_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "../Engine/EngineCore.h"
#include "../Basic/UIWidget.h"

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


UIMainView::UIMainView()
{
	m_Name = "MainView";
	m_Parent = "Commands";
}

UIMainView::~UIMainView()
{}

/*virtual*/ void UIMainView::OnFrameUpdate()
{
	if (ImGui::BeginDock(m_Name.Get()))
	{
		//ImGui::ForceDock(m_Name.Get(), m_Parent.Get(), 1);
	}	
	ImGui::EndDock();
}


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_PREVIEW_H