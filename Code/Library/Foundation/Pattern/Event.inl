// Event
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Pattern/Event.h"

template<typename T, typename... Args>
class EventImpl<T(Args...)>
{
public:
	struct DelegateHandle
	{
	public:
		uint64   m_ID;
		Delegate m_Delegate;

		bool operator == (const DelegateHandle& handle)
		{
			return m_ID == handle.m_ID;
		}
	};

	uint64 Bind(const Delegate & d)
	{
		id = GenerateID();
		m_Delegates.Append({ id, d });
	}

	void Unbind(uint64 id)
	{
		if (m_OrderSensitive)
		{ 
		}
		else
		{
			m_Delegates::Iter iter = m_Delegates.Find()
			m_Delegates.EraseSwap()
		}
	}

	void SetOrderSensitive(bool orderSensitive)
	{
		m_OrderSensitive = orderSensitive;
	}

	void Signal(Args... args) const;

private:
	uint64 GenerateID()
	{
		return ++m_CurMaxID;
	}

	void Compact();

	Array<DelegateHandle>	m_Delegates;
	uint32					m_FreeSlots {0};
	uint64					m_CurMaxID {0};
	bool					m_OrderSensitive {false};
};


// Bind
//------------------------------------------------------------------------------
template<typename T, typename... Args>
Delegate<T(Args...)>::Bind(const std::function<T(Args...)> & function)
{
	m_functinon = function;
}

// Unbind
//------------------------------------------------------------------------------
template<typename T, typename... Args>
Delegate<T(Args...)>::Unbind()
{
	m_functinon.target() = nullptr;
}

// IsValid
//------------------------------------------------------------------------------
template<typename T, typename... Args>
Delegate<T(Args...)>::Valid() const
{
	return m_functinon.target() != nullptr;
}

// Equal
//------------------------------------------------------------------------------
template<typename T, typename... Args>
template<class T2, class Args2...>
Delegate<T(Args...)>::Equal(const std::function<T2(Args2...)> & function) const
{
	return m_functinon == function;
}


// Invoke
//------------------------------------------------------------------------------
template<typename T, typename... Args>
T Delegate<T(Args...)>::Invoke(Args... args) const
{
	ASSERT(m_functinon.target(), "Cannot invoke unbound delegate.");
	return m_functinon();
}


// Bind
//------------------------------------------------------------------------------
template<typename T, typename... Args>
Event<T(Args...)>::Bind(const std::function<T(Args...)> & function)
{
	m_Impl->Bind(function);
}

// Unbind
//------------------------------------------------------------------------------
template<typename T, typename... Args>
Event<T(Args...)>::Unbind(const std::function<T(Args...)> & function)
{
	m_Impl->Unbind(function);
}

// Signal
//------------------------------------------------------------------------------
template<typename T, typename... Args>
Event<T(Args...)>::Signal(Args... args)
{
	m_Impl->Signal(args);
}


// Bind
//------------------------------------------------------------------------------
template<typename T, typename... Args>
Event<T(Args...)>::EventImpl::Bind(const std::function<T(Args...)> & function)
{
	m_Delegates.AppendUnique(function);
}

// Unbind
//------------------------------------------------------------------------------
template<typename T, typename... Args>
Event<T(Args...)>::EventImpl::Unbind(const std::function<T(Args...)> & function)
{
	for (auto & d : m_Delegates)
	{
		if (d.Equal(function))
		{
			d.Unbind();
			++m_FreeSlots;
			break;
		}
	}
}

// Unbind
//------------------------------------------------------------------------------
template<typename T, typename... Args>
Event<T(Args...)>::EventImpl::Unbind(const std::function<T(Args...)> & function)
{
	for (auto & d : m_Delegates)
	{
		if (d.Equal(function))
		{
			d.Unbind();
			++m_FreeSlots;
			break;
		}
	}
}

// Signal
//------------------------------------------------------------------------------
template<typename T, typename... Args>
Event<T(Args...)>::EventImpl::Signal(Args... args) const
{
	for (auto & d : m_Delegates)
	{
		if (d.Valid())
		{
			d.Invoke(args);
		}
	}
}
