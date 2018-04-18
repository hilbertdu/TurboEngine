// OutputPanel.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "AutoPack/UI/Panels/Output.h"
#include "AutoPack/Engine/EngineCore.h"
#include "AutoPack/Event/EventDecl.h"
#include "AutoPack/Service/Service.h"
#include "AutoPack/Service/Command.h"
#include "AutoPack/Engine/EngineCore.h"


// UIOutputPanel
//------------------------------------------------------------------------------
UIOutputPanel::UIOutputPanel()
{
	m_Logger.SetSize(MAX_LOGGER_SIZE);
}

// PollOutput
//------------------------------------------------------------------------------
void UIOutputPanel::PollOutput()
{
	if (!IsValid()) return;

	auto services = EngineCore::Instance().GetServiceManager()->GetServices();
	for (auto item : services)
	{
		for (auto cmd : item->GetCommands())
		{
			if (cmd->GetLock().TryLock())
			{
				ImGuiLogger & logger = ObtainLogger(m_CurService, m_CurCommand);
				const AStringView & str = cmd->FetchOutput(logger.m_StrIndex, 1024);

				if (str.GetLength() > 0)
				{
					logger.m_StrIndex += str.GetLength();
					logger.AddLog(str.Get());
					logger.AddLog("\0");
				}
				cmd->GetLock().UnLock();
			}
		}
	}
}

UIOutputPanel::ImGuiLogger & UIOutputPanel::ObtainLogger(const AStringView & id)
{
	// find used logger
	ImGuiLogger * loggerUsed = m_Logger.FindIf([id](const ImGuiLogger & logger) {return id == logger.m_Id; });
	if (loggerUsed)
	{
		return *loggerUsed;
	}

	// find free logger	
	ImGuiLogger * loggerNew = m_Logger.FindIf([id](const ImGuiLogger & logger) {return !logger.m_Used; });
	if (loggerNew)
	{
		loggerNew->m_Id = id.Get();
		loggerNew->m_Used = true;
		return *loggerNew;
	}
	ASSERT(false);
	abort();
}

UIOutputPanel::ImGuiLogger & UIOutputPanel::ObtainLogger(const AStringView & service, const AStringView & command)
{
	AStackString<64> key = m_CurService + "_" + m_CurCommand;
	return ObtainLogger(key);
}

void UIOutputPanel::ImGuiLogger::AddLog(const char* fmt, ...)
{
	int oldSize = m_Buf.size();
	va_list args;
	va_start(args, fmt);
	m_Buf.appendfv(fmt, args);
	va_end(args);
	for (int newSize = m_Buf.size(); oldSize < newSize; oldSize++)
		if (m_Buf[oldSize] == '\n')
			m_LineOffsets.push_back(oldSize);
	m_ScrollToBottom = true;
}

void UIOutputPanel::ImGuiLogger::Clear()
{
	m_Buf.clear();
	m_LineOffsets.clear();
}

void UIOutputPanel::ImGuiLogger::Draw()
{
	if (ImGui::Button("Clear")) { Clear(); }

	ImGui::SameLine();
	bool copy = ImGui::Button("Copy");
	ImGui::SameLine();
	m_Filter.Draw("Filter", -100.0f);
	ImGui::Separator();
	ImGui::BeginChild("Scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
	if (copy) ImGui::LogToClipboard();

	if (m_Filter.IsActive())
	{
		const char* buf_begin = m_Buf.begin();
		const char* line = buf_begin;
		for (int line_no = 0; line != nullptr; line_no++)
		{
			const char* line_end = (line_no < m_LineOffsets.Size) ? buf_begin + m_LineOffsets[line_no] : nullptr;
			if (m_Filter.PassFilter(line, line_end))
				ImGui::TextUnformatted(line, line_end);
			line = line_end && line_end[1] ? line_end + 1 : nullptr;
		}
	}
	else
	{
		ImGui::TextUnformatted(m_Buf.begin());
	}

	if (m_ScrollToBottom) { ImGui::SetScrollHere(1.0f); }
	m_ScrollToBottom = false;
	ImGui::EndChild();
}

bool UIOutputPanel::IsValid()
{
	return !m_CurService.IsEmpty() && !m_CurCommand.IsEmpty();
}

/*virtual*/ void UIOutputPanel::OnFrameUpdate()
{
	if (ImGui::BeginDock("Output"))
	{
		{
			// service combo
			ImGui::PushItemWidth(120);
			if (ImGui::BeginCombo("Service", m_CurService.Get()))
			{
				for (auto service : EngineCore::Instance().GetServiceManager()->GetServices())
				{
					bool isSelected = (m_CurService == service->GetName());
					if (ImGui::Selectable(service->GetName().Get(), isSelected))
					{
						m_CurService = service->GetName();
					}
					if (isSelected)
					{
						ImGui::SetItemDefaultFocus();
					}
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
			ImGui::SameLine(0, 20);

			// commands combo
			ImGui::PushItemWidth(160);
			if (ImGui::BeginCombo("Command", m_CurCommand.Get()))
			{
				const IService * service = EngineCore::Instance().GetServiceManager()->GetService(m_CurService);
				if (service)
				{
					for (auto command : service->GetCommands())
					{
						bool isSelected = (m_CurCommand == command->GetName());
						if (ImGui::Selectable(command->GetName().Get(), isSelected))
						{
							m_CurCommand = command->GetName();
						}
						if (isSelected)
						{
							ImGui::SetItemDefaultFocus();
						}
					}
				}
				ImGui::EndCombo();
			}
			ImGui::PopItemWidth();
		}

		{
			// draw logger
			if (IsValid())
			{
				ImGuiLogger & logger = ObtainLogger(m_CurService, m_CurCommand);
				logger.Draw();
			}
		}
	}
	ImGui::EndDock();

	PollOutput();
}

//------------------------------------------------------------------------------