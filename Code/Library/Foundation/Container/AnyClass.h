// AnyClass.h
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/Memory/Mem.h"

// AnyClass class
//------------------------------------------------------------------------------


class AnyClass
{
public:
	class IHolder
	{
	public:		
		virtual ~IHolder() {}
		virtual IHolder* Clone() const = 0;
	};

	template<class T>
	class Holder : public IHolder
	{
	public:
		Holder(const T& value) : m_content(value) {}
		virtual IHolder* Clone() const { return TNEW(Holder(m_content)); }

		T m_content;
	};

	AnyClass() : m_holder(nullptr) {}

	template<class T>
	AnyClass(T&& t) : m_holder(TNEW(Holder<T>(std::forward<T>(t)))) {}

	AnyClass(const AnyClass& other)
		: m_holder(nullptr)
	{
		if (other.m_holder)
		{
			m_holder = other.m_holder->Clone();
		}
	}

	AnyClass(AnyClass&& other) : m_holder(other.m_holder) { other.m_holder = nullptr; }

	~AnyClass() { TDELETE_SAFE(m_holder); }

	AnyClass& operator=(AnyClass other)
	{
		Swap(other);
		return *this;
	}

	template<class T>
	AnyClass& operator=(T&& rhs)
	{
		AnyClass(std::forward<T>(rhs)).Swap(*this);
		return *this;
	}

	inline void Swap(AnyClass& other)
	{
		std::swap(m_holder, other.m_holder);
	}

	inline bool IsEmpty() const
	{
		return m_holder == nullptr;
	}

	template<class T>
	inline T* Cast()
	{
		return &static_cast<Holder<T>*>(m_holder)->m_content;
	}

	template<class T>
	inline const T* Cast() const
	{
		return &static_cast<Holder<T>*>(m_holder)->m_content;
	}

private:
	IHolder * m_holder;
};

//------------------------------------------------------------------------------