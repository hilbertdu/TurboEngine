// MainMenu.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_MAINMENU_H
#define MESSIAH_AUTOPACK_MAINMENU_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "../Basic/UIWidget.h"
#include "../Engine/EngineCore.h"

// class MAINMENU
//------------------------------------------------------------------------------
class UIMainMenu : public UIWidget
{
public:
	UIMainMenu() {}

	virtual void OnFrameUpdate();

private:
	bool m_Save;

	TREFLECTION_DECLARE(UIMainMenu, UIWidget)
};


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
			ImGui::MenuItem("Save", nullptr, &m_Save);
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
		ImVec2 size = ImGui::GetWindowSize();
		m_Size.x = size.x;
		m_Size.y = size.y;
		ImGui::EndMainMenuBar();
	}

	if (m_Save)
	{
		EngineCore::Instance().Save();
		m_Save = false;
	}
}

//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_MAINMENU_H