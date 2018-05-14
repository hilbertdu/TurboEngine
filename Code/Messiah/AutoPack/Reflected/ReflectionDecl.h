// ReflectionDecl.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_REFLECTION_DECL_H
#define MESSIAH_AUTOPACK_REFLECTION_DECL_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/ReflectionMacros.h"
#include "AutoPack/UI/Basic/UIWidget.h"
#include "AutoPack/UI/Compat/UIDocker.h"
#include "AutoPack/UI/Panels/MainMenu.h"
#include "AutoPack/UI/Panels/AssetBrowser.h"
#include "AutoPack/UI/Panels/MainToolbar.h"
#include "AutoPack/UI/Panels/GameView.h"
#include "AutoPack/UI/Panels/FileProperty.h"
#include "AutoPack/UI/Panels/Commands.h"
#include "AutoPack/UI/Panels/Output.h"
#include "AutoPack/Service/Service.h"
#include "AutoPack/Service/Command.h"


// Declarations
//------------------------------------------------------------------------------

// UIDocker
//------------------------------------------------------------------------------
TREFLECT_BEGIN(ImGUIDocker)
	TREFLECT_FIELD(m_Idx,		"Idx",			ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_Label,		"Label",		ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_NextTab,	"NextTab",		ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_PrevTab,	"PrevTab",		ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_Children,	"Children",		ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_Parent,	"Parent",		ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_Pos,		"Pos",			ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_Size,		"Size",			ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_Status,	"Status",		ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_Location,	"Location",		ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_Active,	"Active",		ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_Opened,	"Opened",		ATTRIBUTE(AttrSerializer(true)))
TREFLECT_END(ImGUIDocker)

// UIWidget
//------------------------------------------------------------------------------
TREFLECT_BEGIN(UIWidget)
TREFLECT_END(UIWidget)

// UIMainMenu
//------------------------------------------------------------------------------
TREFLECT_BEGIN(UIMainMenu)
	TREFLECT_FIELD(m_ViewState,	"ViewState",	ATTRIBUTE(AttrSerializer(true)))
TREFLECT_END(UIMainMenu)

// UIMainToolBar
//------------------------------------------------------------------------------
TREFLECT_BEGIN(UIMainToolBar)
TREFLECT_END(UIMainToolBar)

// UIAssetBrowser
//------------------------------------------------------------------------------
TREFLECT_BEGIN(UIAssetBrowser)
TREFLECT_END(UIAssetBrowser)

// UIMainView
//------------------------------------------------------------------------------
TREFLECT_BEGIN(UIGameView)
TREFLECT_END(UIGameView)

// UICommands
//------------------------------------------------------------------------------
TREFLECT_BEGIN(UICommands)
TREFLECT_END(UICommands)

// UIFileProperty
//------------------------------------------------------------------------------
TREFLECT_BEGIN(UIFileProperty)
TREFLECT_END(UIFileProperty)

// UIOutputPanel
//------------------------------------------------------------------------------
TREFLECT_BEGIN(UIOutputPanel)
TREFLECT_END(UIOutputPanel)

// IService
//------------------------------------------------------------------------------
TREFLECT_BEGIN(IService)
	TREFLECT_FIELD(m_Name,			"Name",			ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_Description,	"Description",	ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_Author,		"Author",		ATTRIBUTE(AttrSerializer(true)))
TREFLECT_END(IService)

// ICommand
//------------------------------------------------------------------------------
TREFLECT_BEGIN(ICommand)
	TREFLECT_FIELD(m_Name,			"Name",			ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_Description,	"Description",	ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_Author,		"Author",		ATTRIBUTE(AttrSerializer(true)))
TREFLECT_END(ICommand)

// IBatchService
//------------------------------------------------------------------------------
TREFLECT_BEGIN(IBatchService)
TREFLECT_END(IBatchService)

//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_REFLECTION_DECL_H