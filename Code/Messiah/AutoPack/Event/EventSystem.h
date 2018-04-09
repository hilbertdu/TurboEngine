// EventSystem.h
//------------------------------------------------------------------------------
#pragma once
#ifndef MESSIAH_AUTOPACK_EVENTSYSTEM_H
#define MESSIAH_AUTOPACK_EVENTSYSTEM_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Pattern/Event.h"
#include "Foundation/String/String.h"
#include "Foundation/String/StringView.h"
#include "Foundation/Container/HashMap.h"


// Class EventSystem
//------------------------------------------------------------------------------
class EventSystem
{
public:
	template<typename... TArgs>
	void RegisterEvent(const AStringView & name, Event<void(TArgs...)> && event);
	template<typename... TArgs>
	void BindListener(const AStringView & name, const Delegate<void(TArgs...)> & delegate);
	template<typename... TArgs>
	void BindListener(const AStringView & name, Delegate<void(TArgs...)> && delegate);
	template<typename... TArgs>
	void DispatchEvent(const AStringView & name, TArgs... args);

public:
	HashMap<AString, AnyEvent> m_Events;
};


template<typename... TArgs>
void EventSystem::RegisterEvent(const AStringView & name, Event<void(TArgs...)> && event)
{
	ASSERT(m_Events.Find(name.Get()) == m_Events.End());
	m_Events[name.Get()] = std::move(event);
}

template<typename... TArgs>
void EventSystem::BindListener(const AStringView & name, const Delegate<void(TArgs...)> & delegate)
{
	ASSERT(m_Events.Find(name.Get()) != m_Events.End());
	m_Events[name.Get()].Cast<Event<void(TArgs...)>>()->Add(delegate);
}

template<typename... TArgs>
void EventSystem::BindListener(const AStringView & name, Delegate<void(TArgs...)> && delegate)
{
	ASSERT(m_Events.Find(name.Get()) != m_Events.End());
	m_Events[name.Get()].Cast<Event<void(TArgs...)>>()->Add(std::move(delegate));
}

template<typename... TArgs>
void EventSystem::DispatchEvent(const AStringView & name, TArgs... args)
{
	ASSERT(m_Events.Find(name.Get()) != m_Events.End());
	m_Events[name.Get()].Cast<Event<void(TArgs...)>>()->Signal(std::forward<TArgs>(args)...);
}

//------------------------------------------------------------------------------
#endif // MESSIAH_AUTOPACK_EVENTSYSTEM_H
