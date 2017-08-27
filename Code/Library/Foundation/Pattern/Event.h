// Event.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_PATTERN_EVENT_H
#define FOUNDATION_PATTERN_EVENT_H

// Include
//------------------------------------------------------------------------------
#include "Foundation/Pattern/Delegate.h"


// Event
//-----------------------------------------------------------------------------
#include "Foundation/Pattern/Event.inl"

template<typename T>
class Event;

template<typename T, typename... Args>
class Event<T(Args...)>
{
public:
	Event() : m_Impl(TNEW(EventImpl<T(Args...)>())) {}
	uint64 Add(const Delegate<T(Args...)> & d) { return m_Impl->Add(d); }
	void   Remove(uint64 id) { m_Impl->Remove(id); }

	void SetOrderSensitive(bool orderSensitive) { m_Impl->SetOrderSensitive(orderSensitive); }

	void Signal(Args... args) const
	{
		StrongPtr<EventImpl<T(Args...)>> temp = m_Impl;
		temp->Signal(std::forward<Args>(args)...);
	}

private:
	StrongPtr<EventImpl<T(Args...)>> m_Impl;
};


// Signature
//-----------------------------------------------------------------------------
template<typename T>
class Signature;

template<typename T, typename... Args>
class Signature<T(Args...)>
{
public:
	typedef Delegate<T(Args...)> Delegate;
	typedef Event<T(Args...)>    Event;
};

//------------------------------------------------------------------------------
#endif // FOUNDATION_PATTERN_EVENT_H