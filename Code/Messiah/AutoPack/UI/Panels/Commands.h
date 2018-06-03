// Commands.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_COMMANDS_H
#define MESSIAH_AUTOPACK_COMMANDS_H

// Includes
//------------------------------------------------------------------------------
#include "imgui/imgui.h"
#include "Foundation/Platform/Types.h"
#include "Foundation/Container/Array.h"
#include "Foundation/String/String.h"
#include "AutoPack/UI/Basic/UIWidget.h"

// Forward decl
//------------------------------------------------------------------------------
class IService;
class ICommand;


// class UICommands
//------------------------------------------------------------------------------
class UICommands : public UIWidget
{
public:
	UICommands();
	~UICommands();

	virtual void OnFrameUpdate();

private:
	void UpdateService();
	void UpdateCommand();
	void UpdateAddNewService();
	void UpdateRemoveService();
	void UpdateAddNewCommand();
	void UpdateRemoveCommand();

	void OnServiceAddNew();
	void OnServiceRemove();
	void OnCommandAddNew();
	void OnCommandRemove();

	bool CheckExecValid(const AStringView & exec, const AStringView & dir);

	IService * m_FocusedService;
	ICommand * m_FocusedCommand;
	ICommand * m_FocusedCommandAddType;

	TREFLECTION_DECLARE(UICommands, UIWidget)
};


//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_COMMANDS_H