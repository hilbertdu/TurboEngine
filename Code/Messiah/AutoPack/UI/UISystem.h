// UISYSTEM.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_UISYSTEM_H
#define MESSIAH_AUTOPACK_UISYSTEM_H

// Includes
//------------------------------------------------------------------------------
#include "imgui/imgui.h"
#include "Foundation/Memory/Mem.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/Container/HashMap.h"
#include "Foundation/String/StringView.h"
#include "AutoPack/Engine/Serializer.h"


class UIWidget;
namespace TReflection { class ISerializer; }


// class UISystem
//------------------------------------------------------------------------------
class UISystem : public ISaveLoad
{
public:
	typedef HashMap<AString, UIWidget*> WidgetMap;

	UIWidget* CreateWidget(AStringView name);
	void	  DeleteWidget(AStringView name);

public:
	UISystem();
	~UISystem();

	UIWidget* GetWidget(AStringView name);
	bool	  HasWidget(AStringView name);

	void InitGUI();

	void OnFrameStart();
	void OnFrameEnd();

	bool Undo();
	bool Redo();

private:
	virtual bool SaveText(IOStream* stream) const;
	virtual bool LoadText(const IOStream* stream);

	bool SaveDockers(IOStream* stream, TReflection::ISerializer * writer) const;
	bool LoadDockers(const IOStream* stream, TReflection::ISerializer * reader);

	WidgetMap		m_Widgets;
	Array<AString>	m_WidgetNames;
};

//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_UISYSTEM_H