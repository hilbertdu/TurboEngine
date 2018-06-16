// Ref.h
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/Memory/Deletor.h"
#include "Foundation/Container/SmartPtr.h"


// RefProxy class
//------------------------------------------------------------------------------
template<class T>
class RefProxy
{
public:
	explicit RefProxy() {}
	explicit RefProxy(const WeakPtr<T> & wt) : m_WeakPtr(wt) {}

	inline T *		Get()			{ return m_WeakPtr.Get(); }
	inline const T* Get() const		{ return m_WeakPtr.Get(); }	
	inline bool		IsValid() const { return Get() != nullptr; }

private:
	WeakPtr<T> m_WeakPtr;
};


// RefObject class
//------------------------------------------------------------------------------
template<class T>
class RefObject
{
public:
	virtual ~RefObject() { Release(); }

	inline RefProxy<T> GetRefProxy() { MakeHolder(); return RefProxy<T>(m_WeakPtr); }

public:
	inline void IncRef() 
	{ 
		MakeHolder(); 
		m_WeakPtr.m_RefPointHolder->IncPointerRefCount(); 
	}

	inline void DecRef() 
	{
		ASSERT(m_WeakPtr.Get());
		if (m_WeakPtr.m_RefPointHolder->DecPointerRefCount() == 1) 
		{ 
			m_WeakPtr.m_RefPointHolder->SetPointer(nullptr); Destroy(); 
		} 
	}

private:
	inline void MakeHolder()
	{
		if (!m_WeakPtr.Get())
		{
			m_WeakPtr.m_RefPointHolder = TNEW(RefPointerHolder<T>(static_cast<T*>(this)));
		}
	}

	inline void Release()
	{
		if (m_WeakPtr.Get())
		{
			m_WeakPtr.m_RefPointHolder->DecPointerRefCount();
			m_WeakPtr.m_RefPointHolder->SetPointer(nullptr);
		}
	}

	inline virtual void Destroy() { TDELETE(static_cast<T*>(this)); }

	WeakPtr<T> m_WeakPtr;
};

//------------------------------------------------------------------------------