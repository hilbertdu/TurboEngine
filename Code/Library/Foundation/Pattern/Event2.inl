// Event.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_PATTERN_EVENT_H
#define FOUNDATION_PATTERN_EVENT_H

// Include
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/Container/Pair.h"
#include "Foundation/Container/Array.h"
#include <type_traits>
#include <functional>


// Delegate
//------------------------------------------------------------------------------
template<typename T>
class Delegate;

template<typename T, typename... Args>
class Delegate<T(Args...)>
{
public:
	typedef void* InstancePointer;
	typedef T(*FuncPointer)(Args...);
	typedef T(*InnerFuncPointer)(InstancePointer, Args...);
	typedef Pair<InstancePointer, InnerFuncPointer> InvokeStub;

	template<FuncPointer function>
	static T FunctionStub(InstancePointer pointer, Args... args)
	{
		return (function)(args...);
	}

	template<class C, T(C::*function)(Args...)>
	static T MethodStub(InstancePointer pointer, Args... args)
	{
		return (static_cast<C*>(pointer)->*function)(args...);
	}

	Delegate() : m_Stub(nullptr, nullptr) {}

	inline bool operator == (const Delegate& rOther) { return m_Stub == rOther.m_Stub; }

	template<FuncPointer function>
	void Bind()
	{
		m_Stub.First() = nullptr;
		m_Stub.Second() = &FunctionStub<function>;
	}

	template<class C, T(C::*function)(Args...)>
	void Bind(C* c)
	{
		m_Stub.First() = c;
		m_Stub.Second() = &MethodStub<C, function>;
	}

	void Bind(const std::function<void(int)>& function)
	{

	}

	template<FuncPointer function>
	bool Equal() const
	{
		return m_Stub.First() == nullptr && m_Stub.Second() == &FunctionStub<function>;
	}

	template<class C, T(C::*function)(Args...)>
	bool Equal(C* c) const
	{
		return m_Stub.First() == c && m_Stub.Second() == &MethodStub<C, function>;
	}

	void Unbind()
	{
		m_Stub.First() = nullptr;
		m_Stub.Second() = nullptr;
	}

	bool Valid() const
	{
		return m_Stub.Second() != nullptr;
	}

	T Invoke(Args... args) const
	{
		ASSERT(m_Stub.Second(), "Cannot invoke unbound delegate.");
		return m_Stub.Second()(m_Stub.First(), args...);
	}

private:
	InvokeStub m_Stub;
};


// Event
//-----------------------------------------------------------------------------
template<typename T>
class Event;

template<typename T, typename... Args>
class Event<T(Args...)>
{
public:
	typedef Delegate<T(Args...)>                       EDelegate;
	typedef Array<Delegate<T(Args...)>>                EDelegateList;
	typedef typename Delegate<T(Args...)>::FuncPointer FuncPointer;

	Event() = default;

	template<FuncPointer function>
	SIZET Bind()
	{
		EDelegate delegate;
		delegate.Bind<function>();
		return Bind(delegate);
	}

	template<class C, T(C::*function)(Args...)>
	SIZET Bind(C* c)
	{
		EDelegate delegate;
		delegate.Bind<C, function>(c);
		return Bind(delegate);
	}

	void Unbind(size_t index)
	{
		if (index > 0 && index < m_Delegates.GetSize())
		{
			m_Delegates[index].Unbind();
		}
	}

	template<FuncPointer function>
	void Unbind()
	{
		for (EDelegateList::Iter iter = m_Delegates.Begin(); iter != m_Delegates.End(); ++iter)
		{
			if (iter->Equal<function>())
			{
				iter->Unbind();
				return;
			}
		}
	}

	template<class C, T(C::*function)(Args...)>
	void Unbind(C* c)
	{
		for (EDelegateList::Iter iter = m_Delegates.Begin(); iter != m_Delegates.End(); ++iter)
		{
			if (iter->Equal<C, function>(c))
			{
				iter->Unbind();
				return;
			}
		}
	}

	void Signal(Args... args)
	{
		for (EDelegateList::ConstIter iter = m_Delegates.Begin(); iter != m_Delegates.End(); ++iter)
		{
			if (iter->Valid())
			{
				iter->Invoke(args...);
			}
		}
	}

private:
	size_t Bind(const EDelegate& delegate)
	{
		for (EDelegateList::Iter iter = m_Delegates.Begin(); iter != m_Delegates.End(); ++iter)
		{
			if (*iter == delegate)
			{
				return m_Delegates.Index(iter);
			}
			else if (!iter->Valid())
			{
				*iter = delegate;
				return m_Delegates.Index(iter);
			}
		}
		m_Delegates.Append(delegate);
		return m_Delegates.GetSize() - 1;
	}

	Array<EDelegate> m_Delegates;
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