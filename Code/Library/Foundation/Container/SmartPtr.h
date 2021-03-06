// AutoPtr
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_CONTAINERS_SMARTPTR_H
#define FOUNDATION_CONTAINERS_SMARTPTR_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Memory/Deletor.h"
#include "Foundation/Container/Ref.h"


// AutoPtr
//------------------------------------------------------------------------------
template<class T, class Deletor = NewDeletor<T>>
class AutoPtr : public Deletor
{
public:
	explicit inline AutoPtr() : m_Pointer(nullptr) {}
	explicit inline AutoPtr(T * ptr) : m_Pointer(ptr) {}
	inline		   ~AutoPtr() { Delete(m_Pointer); }

	// access the pointer
	inline		 T * Get() { return m_Pointer; }
	inline const T * Get() const { return m_Pointer; }

	// acquire a new pointer
	inline void operator = (T * newPtr) { Delete(m_Pointer); m_Pointer = newPtr; }

	// manually initiate deletion
	inline void Destroy() { Delete(m_Pointer); m_Pointer = nullptr; }

	// free the pointer without deleting it
	inline T * Release() { T * ptr = m_Pointer; m_Pointer = nullptr; return ptr; }

private:
	T * m_Pointer;
};


// StrongPtr
//------------------------------------------------------------------------------
template<class T, class Deletor>
class WeakPtr;

template<class T, class Deletor = NewDeletor<T>>
class StrongPtr : public Deletor
{
public:
	explicit StrongPtr(const Deletor & d = Deletor());
	explicit StrongPtr(T* pointer, const Deletor & d = Deletor());
	explicit StrongPtr(const WeakPtr<T, Deletor>& pointer);

	StrongPtr(const StrongPtr<T, Deletor>& other);
	StrongPtr(StrongPtr<T, Deletor>&& rOther);

	~StrongPtr();

	StrongPtr& operator = (T* other);
	StrongPtr& operator = (StrongPtr<T, Deletor> rOther);
	StrongPtr& operator = (const WeakPtr<T, Deletor>& rOther);

	T& operator* () const  { return *m_RefPointHolder->GetPointer(); }
	T* operator-> () const { return m_RefPointHolder->GetPointer(); }

	inline T*		Get() { return m_RefPointHolder ? m_RefPointHolder->GetPointer() : nullptr; }
	inline const T* Get() const { return m_RefPointHolder ? m_RefPointHolder->GetPointer() : nullptr; }	
	void Set(T* pointer);

	void Swap(StrongPtr& other);

private:
	void AssignHolder(RefPointerHolder<T>* ptrHolder, bool increment = true);
	void Release();
	void ReleaseHolder();

	RefPointerHolder<T>* m_RefPointHolder;

	friend class WeakPtr<T, Deletor>;
};


// WeakPtr
//------------------------------------------------------------------------------
template<class T>
class RefObject;

template<class T, class Deletor = NewDeletor<T>>
class WeakPtr
{
public:
	explicit WeakPtr();
	explicit WeakPtr(const StrongPtr<T, Deletor>& ptr);

	WeakPtr(const WeakPtr<T, Deletor>& other);
	WeakPtr(WeakPtr<T, Deletor>&& rOther);

	~WeakPtr();

	WeakPtr& operator = (WeakPtr<T, Deletor> rOther);
	WeakPtr& operator = (const StrongPtr<T, Deletor>& rOther);

	T& operator* () const { return *m_RefPointHolder->GetPointer(); }
	T* operator-> () const { return m_RefPointHolder->GetPointer(); }

	inline T*		Get() { return m_RefPointHolder ? m_RefPointHolder->GetPointer() : nullptr; }
	inline const T* Get() const { return m_RefPointHolder ? m_RefPointHolder->GetPointer() : nullptr; }

	void Reset();
	bool Expired();
	StrongPtr<T, Deletor> Lock();

	void Swap(WeakPtr& other);

protected:
	void AssignHolder(RefPointerHolder<T>* ptrHolder);
	void ReleaseHolder();

	RefPointerHolder<T>* m_RefPointHolder;

	friend class RefObject<T>;
};


// StrongPtr class implements
//------------------------------------------------------------------------------
template<class T, class Deletor>
/*explicit*/ StrongPtr<T, Deletor>::StrongPtr(const Deletor & d)
	: m_RefPointHolder(nullptr)
	, Deletor(d)
{
}

template<class T, class Deletor>
/*explicit*/ StrongPtr<T, Deletor>::StrongPtr(T* pointer, const Deletor & d)
	: m_RefPointHolder(TNEW(RefPointerHolder<T>(pointer)))
	, Deletor(d)
{
}

template<class T, class Deletor>
/*explicit*/ StrongPtr<T, Deletor>::StrongPtr(const WeakPtr<T, Deletor>& weakPtr)
	: m_RefPointHolder(nullptr)
{
	AssignHolder(weakPtr.m_RefPointHolder);
}

template<class T, class Deletor>
StrongPtr<T, Deletor>::StrongPtr(const StrongPtr<T, Deletor>& rOther)
	: m_RefPointHolder(nullptr)
{
	AssignHolder(rOther.m_RefPointHolder);
}

template<class T, class Deletor>
StrongPtr<T, Deletor>::StrongPtr(StrongPtr<T, Deletor>&& rOther)
	: m_RefPointHolder(rOther.m_RefPointHolder)
{
	rOther.m_RefPointHolder = nullptr;
}

template<class T, class Deletor>
StrongPtr<T, Deletor>::~StrongPtr()
{
	if (m_RefPointHolder)
	{
		Release();
		ReleaseHolder();
	}
}

template<class T, class Deletor>
StrongPtr<T, Deletor>& StrongPtr<T, Deletor>::operator = (T * other)
{
	StrongPtr(other).Swap(*this);
	return *this;
}

template<class T, class Deletor>
StrongPtr<T, Deletor>& StrongPtr<T, Deletor>::operator = (StrongPtr<T, Deletor> rOther)
{
	Swap(rOther);
	return *this;
}

template<class T, class Deletor>
StrongPtr<T, Deletor>& StrongPtr<T, Deletor>::operator = (const WeakPtr<T, Deletor>& rOther)
{
	AssignHolder(rOther.m_RefPointHolder);
}

template<class T, class Deletor>
void StrongPtr<T, Deletor>::Set(T* pointer)
{
	if (!m_RefPointHolder || pointer != m_RefPointHolder->GetPointer())
	{
		AssignHolder(TNEW(RefPointerHolder<T>(pointer)), false);
	}
}

template<class T, class Deletor>
void StrongPtr<T, Deletor>::Swap(StrongPtr& other)
{
	std::swap(m_RefPointHolder, other.m_RefPointHolder);
}

template<class T, class Deletor>
void StrongPtr<T, Deletor>::AssignHolder(RefPointerHolder<T>* ptrHolder, bool increment)
{
	if (m_RefPointHolder)
	{
		Release();
		ReleaseHolder();
	}

	if (ptrHolder)
	{
		m_RefPointHolder = ptrHolder;
		if (increment)
		{
			m_RefPointHolder->IncRefCount();
			m_RefPointHolder->IncPointerRefCount();
		}
	}
}

template<class T, class Deletor>
void StrongPtr<T, Deletor>::Release()
{
	ASSERT(m_RefPointHolder);

	if (m_RefPointHolder->DecPointerRefCount() == 0)
	{
		T* pointer = m_RefPointHolder->GetPointer();
		if (pointer)
		{
			Delete(pointer);
			m_RefPointHolder->SetPointer(nullptr);
		}
	}
}

template<class T, class Deletor>
void StrongPtr<T, Deletor>::ReleaseHolder()
{
	ASSERT(m_RefPointHolder);

	if (m_RefPointHolder->DecRefCount() == 0)
	{
		TDELETE(m_RefPointHolder);
	}
	m_RefPointHolder = nullptr;
}


// WeakPtr class implements
//------------------------------------------------------------------------------
template<class T, class Deletor>
/*explicit*/ WeakPtr<T, Deletor>::WeakPtr()
	: m_RefPointHolder(nullptr)
{
}

template<class T, class Deletor>
/*explicit*/ WeakPtr<T, Deletor>::WeakPtr(const StrongPtr<T, Deletor>& ptr)
	: m_RefPointHolder(nullptr)
{
	AssignHolder(ptr.m_RefPointHolder);
}

template<class T, class Deletor>
WeakPtr<T, Deletor>::WeakPtr(const WeakPtr<T, Deletor>& rOther)
	: m_RefPointHolder(nullptr)
{
	AssignHolder(rOther.m_RefPointHolder);
}

template<class T, class Deletor>
WeakPtr<T, Deletor>::WeakPtr(WeakPtr<T, Deletor>&& rOther)
	: m_RefPointHolder(rOther.m_RefPointHolder)
{
	rOther.m_RefPointHolder = nullptr;
}

template<class T, class Deletor>
WeakPtr<T, Deletor>::~WeakPtr()
{
	Reset();
}

template<class T, class Deletor>
WeakPtr<T, Deletor>& WeakPtr<T, Deletor>::operator = (WeakPtr<T, Deletor> rOther)
{
	Swap(rOther);
	return *this;
}

template<class T, class Deletor>
WeakPtr<T, Deletor>& WeakPtr<T, Deletor>::operator = (const StrongPtr<T, Deletor>& rOther)
{
	AssignHolder(rOther.m_RefPointHolder);
	return *this;
}

template<class T, class Deletor>
void WeakPtr<T, Deletor>::Reset()
{
	if (m_RefPointHolder)
	{
		ReleaseHolder();
	}
}

template<class T, class Deletor>
bool WeakPtr<T, Deletor>::Expired()
{
	return !m_RefPointHolder || m_RefPointHolder->GetPointer() == nullptr;
}

template<class T, class Deletor>
StrongPtr<T, Deletor> WeakPtr<T, Deletor>::Lock()
{
	return StrongPtr(*this);
}

template<class T, class Deletor>
void WeakPtr<T, Deletor>::Swap(WeakPtr& other)
{
	std::swap(m_RefPointHolder, other.m_RefPointHolder);
}

template<class T, class Deletor>
void WeakPtr<T, Deletor>::AssignHolder(RefPointerHolder<T>* ptrHolder)
{
	if (m_RefPointHolder)
	{
		ReleaseHolder();
	}

	if (ptrHolder)
	{
		m_RefPointHolder = ptrHolder;
		m_RefPointHolder->IncRefCount();
	}
}

template<class T, class Deletor>
void WeakPtr<T, Deletor>::ReleaseHolder()
{
	ASSERT(m_RefPointHolder);
	if (m_RefPointHolder->DecRefCount() == 0)
	{
		TDELETE(m_RefPointHolder);
	}
	m_RefPointHolder = nullptr;
}


//------------------------------------------------------------------------------
#endif // FOUNDATION_CONTAINERS_SMARTPTR_H
