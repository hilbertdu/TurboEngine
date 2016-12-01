// AutoPtr
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_CONTAINERS_SMARTPTR_H
#define FOUNDATION_CONTAINERS_SMARTPTR_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Memory/Mem.h"
#include "Foundation/Container/Ref.h"

// DefaultDeletor - free using "Free"
//------------------------------------------------------------------------------
class NewDeletor
{
public:
	static inline void Delete(void * ptr) { TDELETE(ptr); }
};

class ArrayDeletor
{
public:
	static inline void Delete(void * ptr) { TDELETE_ARRAY(ptr); }
};

class AllocDeletor
{
public:
	static inline void Delete(void * ptr) { FREE(ptr); }
};

template<int BLOCK_SIZE, int ALIGNMENT = sizeof(void *)>
class PoolDeletor
{
public:
	static inline void Delete(void * ptr) { MemPool::GetPoolBlock(T_MEM_ALIGN_ARB(BLOCK_SIZE, ALIGNMENT))->Free(ptr); }
};


// AutoPtr
//------------------------------------------------------------------------------
template<class T, class Deletor = NewDeletor>
class AutoPtr
{
public:
	explicit inline AutoPtr() : m_Pointer(nullptr) {}
	explicit inline AutoPtr(T * ptr) : m_Pointer(ptr) {}
	inline		   ~AutoPtr() { Deletor::Delete(m_Pointer); }

	// access the pointer
	inline		 T * Get() { return m_Pointer; }
	inline const T * Get() const { return m_Pointer; }

	// acquire a new pointer
	inline void operator = (T * newPtr) { Deletor::Delete(m_Pointer); m_Pointer = newPtr; }

	// manually initiate deletion
	inline void Destroy() { Deletor::Delete(m_Pointer); m_Pointer = nullptr; }

	// free the pointer without deleting it
	inline T * Release() { T * ptr = m_Pointer; m_Pointer = nullptr; return ptr; }
private:
	T * m_Pointer;
};

//std::shared_ptr
// StrongPtr
//------------------------------------------------------------------------------
template<class T, class Deletor>
class WeakPtr;

template<class T, class Deletor = NewDeletor>
class StrongPtr
{
public:
	explicit StrongPtr();
	explicit StrongPtr(T* pointer);
	explicit StrongPtr(const WeakPtr<T, Deletor>& pointer);

	StrongPtr(const StrongPtr<T, Deletor>& rOther);

	~StrongPtr();

	StrongPtr& operator = (const StrongPtr<T, Deletor>& rOther);
	StrongPtr& operator = (const WeakPtr<T, Deletor>& rOther);

	T& operator* () const  { return m_RefPointHolder->GetPointer(); };
	T* operator-> () const { return *(m_RefPointHolder->GetPointer()); };

	T*   Get() { return m_RefPointHolder->GetPointer(); };
	void Set(T* pointer);

private:
	void AssignHolder(RefPointHolder<T>* ptrHolder, bool increment = true);
	void Release();
	void ReleaseHolder();

	RefPointHolder<T>* m_RefPointHolder;

	friend class WeakPtr<T, Deletor>;
};


// WeakPtr
//------------------------------------------------------------------------------
template<class T, class Deletor = NewDeletor>
class WeakPtr
{
public:
	explicit WeakPtr();
	explicit WeakPtr(const StrongPtr<T, Deletor>& ptr);

	WeakPtr(const WeakPtr<T, Deletor>& rOther);

	~WeakPtr();

	WeakPtr& operator = (const WeakPtr<T, Deletor>& rOther);
	WeakPtr& operator = (const StrongPtr<T, Deletor>& rOther);

	T& operator* () const { return m_RefPointHolder->GetPointer(); };
	T* operator-> () const { return *(m_RefPointHolder->GetPointer()); };

	T* Get() const { return m_RefPointHolder->GetPointer(); };

	void Reset();
	bool Expired();
	StrongPtr<T, Deletor> Lock();

private:
	void AssignHolder(RefPointHolder<T>* ptrHolder);
	void ReleaseHolder();

	RefPointHolder<T>* m_RefPointHolder;
};


// StrongPtr class implements
//------------------------------------------------------------------------------
template<class T, class Deletor>
/*explicit*/ StrongPtr<T, Deletor>::StrongPtr()
	: m_RefPointHolder(nullptr)
{
}

template<class T, class Deletor>
/*explicit*/ StrongPtr<T, Deletor>::StrongPtr(T* pointer)
	: m_RefPointHolder(TNEW(RefPointHolder<T>(pointer)))
{
}

template<class T, class Deletor>
/*explicit*/ StrongPtr<T, Deletor>::StrongPtr(const WeakPtr<T, Deletor>& weakPtr)
{
	AssignHolder(weakPtr.m_RefPointHolder);
}

template<class T, class Deletor>
StrongPtr<T, Deletor>::StrongPtr(const StrongPtr<T, Deletor>& rOther)
{
	AssignHolder(rOther.m_RefPointHolder);
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
StrongPtr<T, Deletor>& StrongPtr<T, Deletor>::operator = (const StrongPtr<T, Deletor>& rOther)
{
	if (this != &rOther)
	{
		AssignHolder(rOther.m_RefPointHolder);
	}
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
		AssignHolder(TNEW(RefPointHolder<T>(pointer)), false);
	}
}

template<class T, class Deletor>
void StrongPtr<T, Deletor>::AssignHolder(RefPointHolder<T>* ptrHolder, bool increment)
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
			Deletor::Delete(pointer);
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
{
	AssignHolder(ptr.m_RefPointHolder);
}

template<class T, class Deletor>
WeakPtr<T, Deletor>::WeakPtr(const WeakPtr<T, Deletor>& rOther)
{
	AssignHolder(rOther.m_RefPointHolder);
}

template<class T, class Deletor>
WeakPtr<T, Deletor>::~WeakPtr()
{
	Reset();
}

template<class T, class Deletor>
WeakPtr<T, Deletor>& WeakPtr<T, Deletor>::operator = (const WeakPtr<T, Deletor>& rOther)
{
	AssignHolder(rOther.m_RefPointHolder);
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
void WeakPtr<T, Deletor>::AssignHolder(RefPointHolder<T>* ptrHolder)
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
