// EventImpl
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Container/Array.h"

#define EVENT_FREE_SLOT_THRETHHOLD 1024


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

		DelegateHandle() : m_ID(0) {}
		DelegateHandle(uint64 id, const Delegate<T(Args...)>& d) : m_ID(id), m_Delegate(d) {}
		DelegateHandle(const DelegateHandle& d) = default;

		DelegateHandle& operator = (const DelegateHandle& rOther)
		{
			m_ID = rOther.m_ID;
			m_Delegate = rOther.m_Delegate;
			return *this;
		}

		DelegateHandle& operator = (DelegateHandle&& rOther)
		{
			m_ID = rOther.m_ID;
			m_Delegate = rOther.m_Delegate;
			return *this;
		}

		bool operator == (const DelegateHandle& handle)
		{
			return m_ID == handle.m_ID;
		}
	};

	uint64 Add(const Delegate<T(Args...)> & d)
	{
		uint64 id = GenerateID();
		m_Delegates.Append(DelegateHandle(id, d));
		return id;
	}

	void Remove(uint64 id)
	{
		auto iter = m_Delegates.FindIf([&id](const DelegateHandle& d) {
			return d.m_ID == id;
		});

		if (iter == nullptr)
		{
			return;
		}

		if (m_OrderSensitive)
		{
			(*iter).m_Delegate.Unbind();
			++m_FreeSlots;
			if (m_FreeSlots > EVENT_FREE_SLOT_THRETHHOLD && (m_FreeSlots << 2) >= m_Delegates.GetSize())
			{
				Compact();
			}
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
		Array<DelegateHandle>::Iter iter1 = m_Delegates.Begin();
		Array<DelegateHandle>::Iter iter2 = m_Delegates.Begin();

		for (; iter2 != m_Delegates.End(); ++iter1, ++iter2)
		{
			while (iter2 != m_Delegates.End() && !(*iter2).m_Delegate.IsValid()) ++iter2;
			if (iter2 == m_Delegates.End()) break;
			(*iter1) = (*iter2);
		}
		m_Delegates.SetSize(m_Delegates.Index(iter1));
	}

	Array<DelegateHandle>	m_Delegates;
	SIZET					m_FreeSlots {0};
	uint64					m_CurMaxID {0};
	bool					m_OrderSensitive {false};
};

//------------------------------------------------------------------------------