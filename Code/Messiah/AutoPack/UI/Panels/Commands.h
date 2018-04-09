// Commands.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_COMMANDS_H
#define MESSIAH_AUTOPACK_COMMANDS_H

// Includes
//------------------------------------------------------------------------------
#include "imgui/imgui.h"
#include "Foundation/Platform/Types.h"
#include "../Basic/UIWidget.h"
#include "../Service/Service.h"


class UICommands : public UIWidget
{
public:
	UICommands();
	~UICommands();

	virtual void OnFrameUpdate();

private:
	void UpdateCommandList();
	void UpdateArguments();	

	IService *	m_FocusedService;	
	AString		m_Spliter;
	float		m_LeftColumeWidth;

	TREFLECTION_DECLARE(UICommands, UIWidget)
};


UICommands::UICommands()
	: m_LeftColumeWidth(128)
{
	m_Name = "Commands";
	m_Spliter.Format("%s_%s", m_Name.Get(), "Splitter");
}

UICommands::~UICommands()
{
}

/*virtual*/ void UICommands::OnFrameUpdate()
{
	if (ImGui::BeginDock(m_Name.Get()))
	{
	}

	float contentW = ImGui::GetContentRegionAvailWidth();
	ImVec2 mainSize(contentW - m_LeftColumeWidth, 0);
	ImVec2 leftSize(m_LeftColumeWidth, 0);
	if (leftSize.x < 10) leftSize.x = 10;
	if (leftSize.x > contentW - 10) leftSize.x = contentW - 10;
	if (mainSize.x < 10) mainSize.x = 10;

	UpdateCommandList();

	ImGui::SameLine();
	ImGui::VSplitter(m_Spliter.Get(), &leftSize);
	m_LeftColumeWidth = leftSize.x;
	ImGui::SameLine();

	UpdateArguments();

	ImGui::EndDock();
}

void UICommands::UpdateCommandList()
{
	ImVec2 size(m_LeftColumeWidth, 0);
	ImGui::BeginChild("CommandList", size);
	ImGui::PushItemWidth(120);

	for (auto & service : EngineCore::Instance().GetServiceManager()->GetServices())
	{
		bool focus = m_FocusedService == service;
		if (ImGui::Selectable(service->GetName().Get(), &focus))
		{
			m_FocusedService = const_cast<IService*>(service);
		}
	}
	ImGui::PopItemWidth();
	ImGui::EndChild();
}

void UICommands::UpdateArguments()
{
	ImGui::BeginChild("Arguments");

	if (m_FocusedService)
	{
		// traverse parameters and show (can use reflection)
		m_FocusedService->RunCommand("TestBat");
	}
		
	ImGui::EndChild();
}


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_COMMANDS_H