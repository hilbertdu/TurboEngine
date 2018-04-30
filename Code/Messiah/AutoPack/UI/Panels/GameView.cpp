// MainView.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "AutoPack/UI/Panels/GameView.h"
#include "AutoPack/Engine/EngineCore.h"
#include "AutoPack/App/Application.h"
#include "AutoPack/Game/MessiahGame.h"
#include <d3d11.h>


// Constructor
//------------------------------------------------------------------------------
UIGameView::UIGameView()
	: m_MGame(TNEW(MessiahGame))
{
	m_Name = "GameView";
}

// Destructor
//------------------------------------------------------------------------------
UIGameView::~UIGameView()
{}

// OnFrameUpdate
//------------------------------------------------------------------------------
/*virtual*/ void UIGameView::OnFrameUpdate()
{
	if (ImGui::BeginDock(m_Name.Get(), &m_Opened))
	{
		ImVec2 windowPos = ImGui::GetWindowPos();
		ImVec2 cursorPos = ImGui::GetCursorPos();
		ImVec2 panelSize = ImGui::GetContentRegionAvail();
		ImVec2 padding = ImGui::GetStyle().WindowPadding;

		if (ImGui::HasDocked(m_Name.Get()))
		{
			padding.x -= 3;
			padding.y -= 1;
		}
		else
		{
			padding.x -= 2;
			padding.y -= 2;
		}

		m_MGame->LoadEmbeddedGame();
		m_MGame->SetWindowPos(windowPos.x + cursorPos.x - padding.x, windowPos.y + cursorPos.y - padding.y, panelSize.x + 2 * padding.x, panelSize.y + 2 * padding.y);
		m_MGame->UpdateGame();
		m_MGame->ShowWindow(true);
	}
	else
	{
		m_MGame->ShowWindow(false);
	}
	ImGui::EndDock();
}


//------------------------------------------------------------------------------