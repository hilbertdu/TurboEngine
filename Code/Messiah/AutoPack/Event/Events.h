// Events.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_EVENTS_H
#define MESSIAH_AUTOPACK_EVENTS_H

// Includes
//------------------------------------------------------------------------------
#include "EventDecl.h"


// UI Event lists
//------------------------------------------------------------------------------
EVENT_DECLARE_FUNCTION_BEGIN

EVENT_DECLARE_BEGIN("UI")
	EVENT_REG("OnAssetFileSelected", void(const char*))
EVENT_DECLARE_END("UI")

/*
EVENT_DECLARE_BEGIN("Command")
	EVENT_REG("OnExeCommandOutput", void(const char*))
EVENT_DECLARE_END("Command")
*/

EVENT_DECLARE_FUNCTION_END

//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_EVENTS_H
