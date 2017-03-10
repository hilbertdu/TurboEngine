// EventImpl
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Container/Array.h"

template<typename T>
class EventImpl;


template<typename T, typename... Args>
class EventImpl<T(Args...)>
{
public:
	struct DelegateHandle
	{
	public:
		uint64   m_ID;
		Delegate<T(Args...)> m_Delegate;

		bool operator == (const DelegateHandle& handle)
		{
			return m_ID == handle.m_ID;
		}
	};

	uint64 Bind(const Delegate & d)
	{
		m_Delegates.Append({ GenerateID(), d });
	}

	void Unbind(uint64 id)
	{
		m_Delegates::Iter iter = m_Delegates.FindIf([&id](const DelegateHandle& d) {
			return d.m_ID == id;
		});

		if (m_OrderSensitive)
		{
			(*iter).m_Delegate.Unbind();
		}
		else
		{
			m_Delegates.EraseSwap(iter);
		}
	}

	void SetOrderSensitive(bool orderSensitive)
	{
		m_OrderSensitive = orderSensitive;
	}

	void Signal(Args... args) const
	{
		for (const auto & d : m_Delegates)
		{
			if (d.m_Delegate.IsValid())
			{
				d.m_Delegate.Invoke(args...);
			}
		}
	}

private:
	uint64 GenerateID()
	{
		uint64 id = m_CurMaxID++;
		if (m_CurMaxID == 0)
		{
			m_CurMaxID = 1;
		}
		return id;
	}

	void Compact()
	{
	}

	Array<DelegateHandle>	m_Delegates;
	uint32					m_FreeSlots {0};
	uint64					m_CurMaxID {0};
	bool					m_OrderSensitive {false};
};

//------------------------------------------------------------------------------