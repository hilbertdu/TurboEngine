// MainToolbar.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "AutoPack/UI/Panels/MainToolbar.h"
#include "AutoPack/Engine/EngineCore.h"


// OnFrameUpdate
//------------------------------------------------------------------------------
void UIMainToolBar::OnFrameUpdate()
{
	auto frame_padding = ImGui::GetStyle().FramePadding;
	float padding = frame_padding.y * 2;
	ImVec4 active_color = ImGui::GetStyle().Colors[ImGuiCol_ButtonActive];
	ImVec4 inactive_color(0, 0, 0, 0);
	ImVec2 toolbar_size(ImGui::GetIO().DisplaySize.x, 24 + padding);

	Vector2 menuSize = EngineCore::Instance().GetUISystem()->GetWidget("UIMainMenu")->GetSize();

	if (ImGui::BeginToolbar("MainToolBar", ImVec2(1, menuSize.y), toolbar_size))
	{
	}
	ImGui::EndToolbar();
	m_Size.x = menuSize.x;
	m_Size.y = 24 + padding;

	if (ImGui::GetIO().DisplaySize.y > 0)
	{
		auto pos = ImVec2(0, menuSize.y + m_Size.y);
		auto size = ImGui::GetIO().DisplaySize;
		size.y -= pos.y;
		ImGui::RootDock(pos, size);
	}
}

//------------------------------------------------------------------------------