// Output.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_OUTPUT_PANEL_H
#define MESSIAH_AUTOPACK_OUTPUT_PANEL_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "AutoPack/UI/Basic/UIWidget.h"


#define MAX_LOGGER_SIZE 32


// Class UIOutputPanel
//------------------------------------------------------------------------------
class UIOutputPanel : public UIWidget
{
public:
	struct ImGuiLogger
	{
	public:
		ImGuiLogger() : m_StrIndex(0), m_Used(false) {}
		ImGuiLogger(const AStringView & id) : m_Id(id.Get()), m_StrIndex(0) {}
		void AddLog(const char * fmt, ...);
		void Clear();
		void Draw();

		AStackString<64>	m_Id;
		ImGuiTextBuffer		m_Buf;
		ImGuiTextFilter		m_Filter;
		ImVector<int>		m_LineOffsets;
		bool				m_ScrollToBottom;
		uint32				m_StrIndex;
		bool				m_Used;
	};

	UIOutputPanel();

	virtual void OnFrameUpdate();
	bool IsValid();

private:
	ImGuiLogger & ObtainLogger(const AStringView & id);
	ImGuiLogger & ObtainLogger(const AStringView & service, const AStringView & command);
	void PollOutput();	
	
	AString m_CurService;
	AString	m_CurCommand;
	StackArray<ImGuiLogger, MAX_LOGGER_SIZE> m_Logger;

	TREFLECTION_DECLARE(UIOutputPanel, UIWidget)
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_OUTPUT_PANEL_H