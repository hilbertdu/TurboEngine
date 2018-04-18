// MainView.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "AutoPack/UI/Panels/MainView.h"
#include "AutoPack/Engine/EngineCore.h"


// Constructor
//------------------------------------------------------------------------------
UIMainView::UIMainView()
{
	m_Name = "MainView";
	m_Parent = "Commands";
}

// Destructor
//------------------------------------------------------------------------------
UIMainView::~UIMainView()
{}

// OnFrameUpdate
//------------------------------------------------------------------------------
/*virtual*/ void UIMainView::OnFrameUpdate()
{
	if (ImGui::BeginDock(m_Name.Get()))
	{
	}
	ImGui::EndDock();
}


//------------------------------------------------------------------------------