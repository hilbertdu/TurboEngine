// MainMenu.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "AutoPack/UI/Panels/MainMenu.h"
#include "AutoPack/Engine/EngineCore.h"
#include "AutoPack/Event/EventDecl.h"


// OnFrameUpdate
//------------------------------------------------------------------------------
void UIMainMenu::OnFrameUpdate()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			ImGui::MenuItem("Main menu bar", nullptr);
			ImGui::MenuItem("Console", nullptr);
			ImGui::MenuItem("Log", nullptr);
			ImGui::MenuItem("Simple layout", nullptr);
			ImGui::MenuItem("Property editor", nullptr);
			ImGui::MenuItem("Long text display", nullptr);
			ImGui::MenuItem("Save", nullptr, &m_Saved);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Edit"))
		{
			ImGui::MenuItem("Auto-resizing window", nullptr);
			ImGui::MenuItem("Constrained-resizing window", nullptr);
			ImGui::MenuItem("Simple overlay", nullptr);
			ImGui::MenuItem("Manipulating window titles", nullptr);
			ImGui::MenuItem("Custom rendering", nullptr);
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Tools"))
		{
			ImGui::MenuItem("Add service", nullptr, &m_ServiceAdded);
			ImGui::EndMenu();
		}
		ImVec2 size = ImGui::GetWindowSize();
		m_Size.x = size.x;
		m_Size.y = size.y;
		ImGui::EndMainMenuBar();
	}

	if (m_Saved)
	{
		EngineCore::Instance().Save();
		m_Saved = false;
	}

	if (m_ServiceAdded)
	{
		EVENT_DISPATCH(UI, OnMenuServiceAdded);
		m_ServiceAdded = false;
	}
}

//------------------------------------------------------------------------------