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
		if (ImGui::Button("remove##service")) { OnServiceRemove(); }
		UpdateRemoveService();
		ImGui::SameLine();
		if (ImGui::Button("add...##service")) { OnServiceAddNew(); }
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
		if (ImGui::Button("remove##command")) { OnCommandRemove(); }
		UpdateRemoveCommand();
		ImGui::SameLine();
		if (ImGui::Button("add...##command")) { OnCommandAddNew(); }
		UpdateAddNewCommand();

		if (m_FocusedCommand)
		{
			ImGui::Text("Executable:");
			ImGui::SameLine(80);
			auto exec = m_FocusedCommand->GetProperty<AString>("Executable");
			ImGui::Text(exec.Get());

			ImGui::Text("Arguments:");
			ImGui::SameLine(80);
			auto args = m_FocusedCommand->GetProperty<AString>("Parameters");
			ImGui::Text(args.Get());

			ImGui::Text("Description:");
			ImGui::SameLine(80);
			auto desc = m_FocusedCommand->GetDescription();
			ImGui::Text(desc.IsEmpty() ? "No description" : desc.Get());

			ImGui::Text("Author:");
			ImGui::SameLine(80);
			auto author = m_FocusedCommand->GetAuthor();
			ImGui::Text(author.IsEmpty() ? "No author" : author.Get());

			// TODO: show task state
			if (ImGui::Button("run##command", ImVec2(60, 0))) 
			{ 
				m_FocusedService->RunCommand(m_FocusedCommand->GetName());
				//m_FocusedCommand->Run();
			}
			ImGui::SameLine(80);
			if (ImGui::Button("cancel##command", ImVec2(60, 0))) 
			{ 
				m_FocusedCommand->Cancel(); 
			}
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
		bool valid = true;
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
				name[0] = description[0] = author[0] = '\0';

				m_FocusedService = service;
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
		if (ImGui::Button("Cancel", ImVec2(120, 0))) 
		{ 
			ImGui::CloseCurrentPopup(); 
			name[0] = description[0] = author[0] = '\0';
		}
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
		ImGui::Text("Name:");
		ImGui::SameLine(80);
		static char name[64]{ 0 };
		ImGui::InputText("##InputCommand", name, IM_ARRAYSIZE(name));

		ImGui::Text("Exec Path:");
		ImGui::SameLine(80);
		static char execPath[256]{ 0 };
		ImGui::InputText("##InputCmdExecPath", execPath, IM_ARRAYSIZE(execPath));

		// Parameters
		ImGui::Text("Parameters:");
		ImGui::SameLine(80);
		static char parameter[256]{ 0 };
		ImGui::InputText("##InputCmdParameter", parameter, IM_ARRAYSIZE(parameter));

		ImGui::Text("Description:");
		ImGui::SameLine(80);
		static char description[2048]{ 0 };
		ImGui::InputTextMultiline("##InputDescription", description, IM_ARRAYSIZE(description));

		ImGui::Text("Author:");
		ImGui::SameLine(80);
		static char author[64]{ 0 };
		ImGui::InputText("##InputAuthor", author, IM_ARRAYSIZE(author));

		ImGui::Separator();
		static AStackString<64> errorMsg;
		if (ImGui::Button("OK", ImVec2(120, 0)))
		{
			bool valid = true;
			AStringView dir("");
			if (StringHelper::StrLen(name) == 0 || StringHelper::StrLen(execPath) == 0)
			{
				valid = false;
				errorMsg = "Please input a valid name or exec path!";
			}
			else if (!CheckExecValid(execPath, ""))
			{
				if (CheckExecValid(execPath, EngineCore::Instance().GetRootDir()))
				{
					dir = EngineCore::Instance().GetRootDir();
				}
				else
				{
					valid = false;
					errorMsg = "Executable file is not exists!";
				}
			}

			if (valid)
			{
				ICommand * command = m_FocusedService->CreateCommand(name);
				command->SetParameters(parameter);
				command->SetExecutable(execPath);
				command->SetWorkingDir(dir);
				command->SetDescription(description);
				command->SetAuthor(author);				

				ImGui::CloseCurrentPopup();
				name[0] = execPath[0] = parameter[0] = description[0] = author[0] = '\0';

				m_FocusedCommand = command;
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
		if (ImGui::Button("Cancel", ImVec2(120, 0))) 
		{ 
			ImGui::CloseCurrentPopup(); 
			name[0] = execPath[0] = parameter[0] = description[0] = author[0] = '\0';
		}
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

// CheckExecValid
//------------------------------------------------------------------------------
bool UICommands::CheckExecValid(const AStringView & exec, const AStringView & dir)
{
	if (dir.IsEmpty())
	{ 
		return FileIO::FileExists(exec.Get());
	}
	else
	{
		return FileIO::FileExists((dir + "\\" + exec).Get());
	}
}

//------------------------------------------------------------------------------