// MainMenu.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "AutoPack/UI/Panels/MainMenu.h"
#include "AutoPack/Engine/EngineCore.h"
#include "AutoPack/Event/EventDecl.h"


// UIMainMenu
//------------------------------------------------------------------------------
UIMainMenu::UIMainMenu()
{
	m_ViewState["UIAssetBrowser"] = false;
	m_ViewState["UIFileProperty"] = false;
	m_ViewState["UIOutputPanel"] = false;
	m_ViewState["UICommands"] = false;
	m_ViewState["UIProfiler"] = false;
}

// OnFrameUpdate
//------------------------------------------------------------------------------
void UIMainMenu::OnFrameUpdate()
{
	DoPreActions();
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))			{ ActionSave(); };
			if (ImGui::MenuItem("Save As"))			{ ActionSaveAs(); };
			if (ImGui::MenuItem("Load"))			{ ActionLoad(); };
			if (ImGui::MenuItem("Recent"))			{ ActionRecent(); };
			ImGui::Separator();
			if (ImGui::MenuItem("Settings..."))		{ ActionSettings(); };
			ImGui::Separator();
			if (ImGui::MenuItem("Exit", "Alt+F4"))	{ ActionExit(); };
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			if (ImGui::MenuItem("Undo")) {};
			if (ImGui::MenuItem("Redo")) {};
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Views"))
		{			
			ImGui::MenuItem("Asset Browser", nullptr, &m_ViewState["UIAssetBrowser"]);
			ImGui::MenuItem("Logger", nullptr, &m_ViewState["UIOutputPanel"]);
			ImGui::MenuItem("Commands", nullptr, &m_ViewState["UICommands"]);
			ImGui::MenuItem("Property", nullptr, &m_ViewState["UIFileProperty"]);
			ImGui::Separator();
			if (ImGui::BeginMenu("Games"))
			{
				// should support multi game windows
				ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Debug"))
		{
			ImGui::MenuItem("Profiler", nullptr, &m_ViewState["UIProfiler"]);
			ImGui::EndMenu();
		}
		ImVec2 size = ImGui::GetWindowSize();
		m_Size.x = size.x;
		m_Size.y = size.y;
		ImGui::EndMainMenuBar();
	}
	DoPostActions();	
}

// DoPreActions
//------------------------------------------------------------------------------
void UIMainMenu::DoPreActions()
{	
}

// DoPostActions
//------------------------------------------------------------------------------
void UIMainMenu::DoPostActions()
{
	for (HashMap<AString, bool>::ConstIter iter = m_ViewState.Begin(); iter != m_ViewState.End(); ++iter)
	{
		if (EngineCore::Instance().GetUISystem()->HasWidget(iter->First()))
		{
			EngineCore::Instance().GetUISystem()->GetWidget(iter->First())->SetOpened(iter->Second());
		}
	}
}

// ActionSave
//------------------------------------------------------------------------------
void UIMainMenu::ActionSave()
{
	EngineCore::Instance().Save();
}

// ActionSaveAs
//------------------------------------------------------------------------------
void UIMainMenu::ActionSaveAs()
{}

// ActionLoad
//------------------------------------------------------------------------------
void UIMainMenu::ActionLoad()
{}

// ActionRecent
//------------------------------------------------------------------------------
void UIMainMenu::ActionRecent()
{}

// ActionSettings
//------------------------------------------------------------------------------
void UIMainMenu::ActionSettings()
{}

// ActionExit
//------------------------------------------------------------------------------
void UIMainMenu::ActionExit()
{}

//------------------------------------------------------------------------------