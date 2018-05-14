// CommandAdd.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "AutoPack/UI/Popups/CommandAdd.h"
#include "AutoPack/Engine/EngineCore.h"
#include "AutoPack/Service/Service.h"


// Constructor
//------------------------------------------------------------------------------
UICommandAdd::UICommandAdd()
{
	m_Name = "Add Command";
}

// Destructor
//------------------------------------------------------------------------------
UICommandAdd::~UICommandAdd()
{
}

// OnFrameUpdate
//------------------------------------------------------------------------------
/*virtual*/ void UICommandAdd::OnFrameUpdate()
{	
	if (ImGui::BeginPopupModal(m_Name.Get(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::CollapsingHeader("Service", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Name:");
			ImGui::SameLine();

			// default select first service
			auto services = EngineCore::Instance().GetServiceManager()->GetServices();
			m_ServiceName = services.IsEmpty() ? "" : services[0]->GetName();
			if (ImGui::BeginCombo("##ServiceList", m_ServiceName.Get()))
			{
				for (auto service : EngineCore::Instance().GetServiceManager()->GetServices())
				{
					bool isSelected = (m_ServiceName == service->GetName());
					if (ImGui::Selectable(service->GetName().Get(), isSelected))
					{
						m_ServiceName = service->GetName();
					}
					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			ImGui::SameLine();
			if (ImGui::Button("remove")) {}
			ImGui::SameLine();
			if (ImGui::Button("add...")) {}
		}

		if (ImGui::CollapsingHeader("Command", ImGuiTreeNodeFlags_DefaultOpen))
		{
			ImGui::Text("Name:");
		}

		if (ImGui::Button("OK", ImVec2(120, 0))) 
		{
			ImGui::CloseCurrentPopup(); 
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}
}

//------------------------------------------------------------------------------