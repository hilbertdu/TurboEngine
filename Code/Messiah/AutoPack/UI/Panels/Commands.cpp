// Commands.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "AutoPack/UI/Panels/Commands.h"
#include "AutoPack/Service/Service.h"
#include "AutoPack/Service/Command.h"
#include "AutoPack/Engine/EngineCore.h"
#include "AutoPack/Event/EventDecl.h"


// Constructor
//------------------------------------------------------------------------------
UICommands::UICommands()
	: m_FocusedService(nullptr)
	, m_FocusedCommand(nullptr)
{
	m_Name = "Commands";
}

// Destructor
//------------------------------------------------------------------------------
UICommands::~UICommands()
{
}

/*virtual*/ void UICommands::OnFrameUpdate()
{
	if (ImGui::BeginDock(m_Name.Get(), &m_Opened))
	{
		UpdateService();
		UpdateCommand();
	}
	ImGui::EndDock();	
}

void UICommands::UpdateService()
{
	if (ImGui::CollapsingHeader("Service", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushItemWidth(-120);
		ImGui::Text("Name:");
		ImGui::SameLine(80);

		// default select first service
		auto services = EngineCore::Instance().GetServiceManager()->GetServices();
		if (ImGui::BeginCombo("##ServiceList", m_FocusedService ? m_FocusedService->GetName().Get() : ""))
		{
			for (auto service : services)
			{
				bool isSelected = (m_FocusedService == service);
				if (ImGui::Selectable(service->GetName().Get(), isSelected))
				{
					m_FocusedService = service;
				}
				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		if (ImGui::Button("remove")) { OnServiceRemove(); }
		UpdateRemoveService();
		ImGui::SameLine();
		if (ImGui::Button("add...")) { OnServiceAddNew(); }
		UpdateAddNewService();

		if (m_FocusedService)
		{
			ImGui::Text("Description:");
			ImGui::SameLine(80);
			auto desc = m_FocusedService->GetDescription();
			ImGui::Text(desc.IsEmpty() ? "No description" : desc.Get());

			ImGui::Text("Author:");
			ImGui::SameLine(80);
			auto author = m_FocusedService->GetAuthor();
			ImGui::Text(author.IsEmpty() ? "No author" : author.Get());
		}
		ImGui::PopItemWidth();
	}
}

void UICommands::UpdateCommand()
{
	if (!m_FocusedService) { return; }
	if (ImGui::CollapsingHeader("Command", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::PushItemWidth(-120);
		ImGui::Text("Name:");
		ImGui::SameLine(80);		

		// default select first service
		auto commands = m_FocusedService->GetCommands();
		if (ImGui::BeginCombo("##CommandList", m_FocusedCommand ? m_FocusedCommand->GetName().Get() : ""))
		{
			for (auto command : commands)
			{
				bool isSelected = (m_FocusedCommand == command);
				if (ImGui::Selectable(command->GetName().Get(), isSelected))
				{
					m_FocusedCommand = command;
				}
				if (isSelected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		ImGui::SameLine();
		if (ImGui::Button("remove")) { OnServiceRemove(); }
		ImGui::SameLine();
		if (ImGui::Button("add...")) { OnServiceAddNew(); }

		if (m_FocusedCommand)
		{
			ImGui::Text("Description:");
			ImGui::SameLine(80);
			auto desc = m_FocusedCommand->GetDescription();
			ImGui::Text(desc.IsEmpty() ? "No description" : desc.Get());

			ImGui::Text("Author:");
			ImGui::SameLine(80);
			auto author = m_FocusedCommand->GetAuthor();
			ImGui::Text(author.IsEmpty() ? "No author" : author.Get());
		}
		ImGui::PopItemWidth();
	}
}

void UICommands::UpdateAddNewService()
{
	if (ImGui::BeginPopupModal("Add New Service", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Name:");
		ImGui::SameLine(80);

		static char name[64]{ 0 };
		ImGui::InputText("##InputService", name, IM_ARRAYSIZE(name));
		
		ImGui::Text("Description:");
		ImGui::SameLine(80);

		static char description[2048]{ 0 };
		ImGui::InputTextMultiline("##InputDescription", description, IM_ARRAYSIZE(description));

		ImGui::Text("Author:");
		ImGui::SameLine(80);

		static char author[64]{ 0 };
		ImGui::InputText("##InputAuthor", author, IM_ARRAYSIZE(author));

		ImGui::Separator();
		static bool valid = true;
		static AStackString<64> errorMsg;
		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			if (StringHelper::StrLen(name) == 0)  { errorMsg = "Please input a valid name!"; valid = false; }
			else if (EngineCore::Instance().GetServiceManager()->GetService(name)) {  errorMsg = "Already has a service with the same name!"; valid = false; }
			else { valid = true; }

			if (valid)
			{
				IService * service = EngineCore::Instance().GetServiceManager()->CreateService(name);
				service->SetDescription(description);
				service->SetAuthor(author);
				ImGui::CloseCurrentPopup();
			}
			else
			{
				ImGui::OpenPopup("Error Input");
			}
		}

		if (ImGui::BeginPopupModal("Error Input", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text(errorMsg.Get());
			if (ImGui::Button("Close"))
			{
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndPopup();
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}
}

void UICommands::UpdateRemoveService()
{
	if (ImGui::BeginPopupModal("Remove Service?", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		ImGui::Text("Service {%s} will be deleted.\nThis operation cannot be undone!\n\n", m_FocusedService->GetName().Get());
		ImGui::Separator();

		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			EngineCore::Instance().GetServiceManager()->RemoveService(m_FocusedService->GetName());
			m_FocusedService = nullptr;
			ImGui::CloseCurrentPopup();
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}
}

void UICommands::UpdateAddNewCommand()
{
	if (ImGui::BeginPopupModal("Add New Command", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	{
		if (ImGui::BeginCombo("##AddCommandTypeList", m_FocusedService ? m_FocusedService->GetName().Get() : ""))
		{
			//if (ImGui::Selectable(command->GetName().Get(), isSelected))
		}

		ImGui::Text("Name:");
		ImGui::SameLine(80);

		static char name[64]{ 0 };
		ImGui::InputText("##InputCommand", name, IM_ARRAYSIZE(name));

		ImGui::Text("Description:");
		ImGui::SameLine(80);

		static char description[2048]{ 0 };
		ImGui::InputTextMultiline("##InputDescription", description, IM_ARRAYSIZE(description));

		ImGui::Text("Author:");
		ImGui::SameLine(80);

		static char author[64]{ 0 };
		ImGui::InputText("##InputAuthor", author, IM_ARRAYSIZE(author));

		ImGui::Separator();
		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			//ICommand * command = m_FocusedService->CreateCommand();
			//service->SetDescription(description);
			//service->SetAuthor(author);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0))) { ImGui::CloseCurrentPopup(); }
		ImGui::EndPopup();
	}
}

void UICommands::UpdateRemoveCommand()
{

}

void UICommands::OnServiceAddNew()
{
	ImGui::OpenPopup("Add New Service");
}

void UICommands::OnServiceRemove()
{
	if (m_FocusedService)
	{
		ImGui::OpenPopup("Remove Service?");
	}
}

void UICommands::OnCommandAddNew()
{
	ImGui::OpenPopup("Add New Command");
}

void UICommands::OnCommandRemove()
{
	if (m_FocusedCommand)
	{
		ImGui::OpenPopup("Remove Command?");
	}
}

//------------------------------------------------------------------------------