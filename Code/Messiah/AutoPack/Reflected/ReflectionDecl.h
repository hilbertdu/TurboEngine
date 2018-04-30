// ReflectionDecl.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_REFLECTION_DECL_H
#define MESSIAH_AUTOPACK_REFLECTION_DECL_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Reflection/ReflectionMacros.h"
#include "../UI/Basic/UIWidget.h"
#include "../UI/Compat/UIDocker.h"
#include "../UI/Panels/MainMenu.h"
#include "../UI/Panels/AssetBrowser.h"
#include "../UI/Panels/MainToolbar.h"
#include "../UI/Panels/GameView.h"
#include "../UI/Panels/FileProperty.h"
#include "../UI/Panels/Commands.h"
#include "../UI/Panels/Output.h"
#include "../Service/Service.h"


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
TREFLECT_END(ImGUIDocker)

// UIWidget
//------------------------------------------------------------------------------
TREFLECT_BEGIN(UIWidget)
	TREFLECT_FIELD(m_Name,		"Name",			ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_Pos,		"Pos",			ATTRIBUTE(AttrSerializer(true)))
	TREFLECT_FIELD(m_Size,		"Size",			ATTRIBUTE(AttrSerializer(true)))
TREFLECT_END(UIWidget)

// UIMainMenu
//------------------------------------------------------------------------------
TREFLECT_BEGIN(UIMainMenu)
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
TREFLECT_END(IService)

// IBatchService
//------------------------------------------------------------------------------
TREFLECT_BEGIN(IBatchService)
TREFLECT_END(IBatchService)

//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_REFLECTION_DECL_H