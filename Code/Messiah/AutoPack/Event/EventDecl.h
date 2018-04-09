// EventDecl.h

//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_EVENTDECL_H
#define MESSIAH_AUTOPACK_EVENTDECL_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/String/String.h"
#include "Foundation/Template/Macros.h"
#include "../Engine/EngineCore.h"


// Defines
//------------------------------------------------------------------------------
#define EVENT_DECLARE_FUNCTION_BEGIN	\
	inline void RegisterAllEvents()		\
	{									\
		AStackString<> cate;			\
		AStackString<> eventName;

#define EVENT_DECLARE_BEGIN(category) \
		cate = category;

#define EVENT_REG(name, signature) \
		eventName = cate;																		\
		eventName += name;																		\
		EngineCore::Instance().GetEventSystem()->RegisterEvent(eventName, Event<signature>());

#define EVENT_DECLARE_END(category)	\
		ASSERT(cate == category);

#define EVENT_DECLARE_FUNCTION_END	\
	}

#define EVENT_DISPATCH(category, name, ...) \
	EngineCore::Instance().GetEventSystem()->DispatchEvent(MACRO_STR(MACRO_CAT(category, name)), __VA_ARGS__);

#define EVENT_BIND(category, name, listener) \
	EngineCore::Instance().GetEventSystem()->BindListener(MACRO_STR(MACRO_CAT(category, name)), listener);

//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_EVENTDECL_H
