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
	m_ItemState["UIAssetBrowser"] = false;
	m_ItemState["UIFileProperty"] = false;
	m_ItemState["UIOutputPanel"] = false;
	m_ItemState["UICommands"] = false;
	m_ItemState["UISettings"] = false;

	m_ItemState["UIProfiler"] = false;
}

// OnFrameUpdate
//------------------------------------------------------------------------------
void UIMainMenu::OnFrameUpdate()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))	{ ActionSave(); };
			if (ImGui::MenuItem("Save As")) { ActionSaveAs(); };
			if (ImGui::MenuItem("Load"))	{ ActionLoad(); };
			if (ImGui::MenuItem("Recent"))	{ ActionRecent(); };
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
			ImGui::MenuItem("Asset Browser", nullptr, &m_ItemState["UIAssetBrowser"]);
			ImGui::MenuItem("Logger", nullptr, &m_ItemState["UIOutputPanel"]);
			ImGui::MenuItem("Commands", nullptr, &m_ItemState["UICommands"]);
			ImGui::MenuItem("Property", nullptr, &m_ItemState["UIFileProperty"]);
			ImGui::MenuItem("Settings", nullptr, &m_ItemState["UISettings"]);
			ImGui::Separator();
			ImGui::MenuItem("Game", nullptr);		// should support multi game windows
			ImGui::Separator();
			ImGui::MenuItem("Profiler", nullptr, &m_ItemState["UIProfiler"]);

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Options"))
		{
			if (ImGui::MenuItem("Add Command")) {};
			ImGui::EndMenu();
		}
		ImVec2 size = ImGui::GetWindowSize();
		m_Size.x = size.x;
		m_Size.y = size.y;
		ImGui::EndMainMenuBar();
	}

	for (HashMap<AString, bool>::ConstIter iter = m_ItemState.Begin(); iter != m_ItemState.End(); ++iter)
	{
		if (EngineCore::Instance().GetUISystem()->HasWidget(iter->First()))
		{
			EngineCore::Instance().GetUISystem()->GetWidget(iter->First())->SetOpened(iter->Second());
		}
	}	
}

// ActionAddCommand
//------------------------------------------------------------------------------
void UIMainMenu::ActionAddCommand()
{
	EVENT_DISPATCH(UI, OnMenuServiceAdded);
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

//------------------------------------------------------------------------------