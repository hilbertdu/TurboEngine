// Commands.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_COMMANDS_H
#define MESSIAH_AUTOPACK_COMMANDS_H

// Includes
//------------------------------------------------------------------------------
#include "imgui/imgui.h"
#include "Foundation/Platform/Types.h"
#include "AutoPack/UI/Basic/UIWidget.h"

// Forward decl
//------------------------------------------------------------------------------
class IService;


// class UICommands
//------------------------------------------------------------------------------
class UICommands : public UIWidget
{
public:
	UICommands();
	~UICommands();

	virtual void OnFrameUpdate();	

private:
	void UpdateCommandList();
	void UpdateArguments();	

	void OnServiceAdd();

	IService *	m_FocusedService;	
	AString		m_Spliter;
	float		m_LeftColumeWidth;

	TREFLECTION_DECLARE(UICommands, UIWidget)
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_COMMANDS_H