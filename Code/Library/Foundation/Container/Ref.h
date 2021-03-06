// Ref.h
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"


// RefCount class
//------------------------------------------------------------------------------
class RefCount
{
public:
	explicit RefCount(): m_RefCount(1) {}
	explicit RefCount(int32 count) : m_RefCount(count) {}

	FORCE_INLINE int32 GetRefCount() const { return m_RefCount; }
	FORCE_INLINE int32 IncRefCount() { return ++m_RefCount; }
	FORCE_INLINE int32 DecRefCount() { return --m_RefCount; }

protected:
	FORCE_INLINE void  SetRefCount(int32 count) { m_RefCount = count; }

	volatile int32 m_RefCount;
};


// RefPointerHolder class
//------------------------------------------------------------------------------
template<class T>
class RefPointerHolder: public RefCount
{
public:
	explicit RefPointerHolder() : m_Pointer(nullptr), m_RefPointerCount(0) {};
	explicit RefPointerHolder(T* pointer) : m_Pointer(pointer), m_RefPointerCount(1) {};

	FORCE_INLINE T* GetPointer() { return m_Pointer; };
	FORCE_INLINE void SetPointer(T* pointer) { m_Pointer = pointer; m_RefPointerCount = 1; };

	FORCE_INLINE int32 GetPointerRefCount() { return m_RefPointerCount; };

	FORCE_INLINE int32 IncPointerRefCount() { return ++m_RefPointerCount; };
	FORCE_INLINE int32 DecPointerRefCount() { return --m_RefPointerCount; };

private:
	T* m_Pointer;
	volatile int32 m_RefPointerCount;
};


// Ref class
//------------------------------------------------------------------------------
template<class T>
class Ref
{
public:
	inline Ref() : m_Pointer(nullptr) {}	
	inline Ref(T * ptr) : m_Pointer(ptr) { if (ptr) { ptr->IncRef(); } }
	Ref(const Ref<T> & other) : m_Pointer(other.Get()) { if (m_Pointer) { m_Pointer->IncRef(); } }
	Ref(Ref<T> && other) : m_Pointer(other.m_Pointer) { other.m_Pointer = nullptr; }
	inline ~Ref() { T * ptr = m_Pointer; if (ptr) { ptr->DecRef(); } }

	// access the pointer
	inline T * Get() { return m_Pointer; }
	inline T * Get() const { return m_Pointer; }
	inline T * operator ->() { return m_Pointer; }
	inline T * operator ->() const { return m_Pointer; }

	inline Ref & operator = (Ref other) { std::swap(m_Pointer, other.m_Pointer); return *this; }

	template<class U>
	inline void operator = (Ref<U> other) { std::swap(m_Pointer, other.m_Pointer); }
	template<class U>
	inline void operator = (U * other) { Assign(other); }

	inline bool operator == (const Ref<T> & other) const { return m_Pointer == other.m_Pointer; }
	inline bool operator == (T * other) const { return m_Pointer == other; }

	inline T* const & GetPointerRef() const { return m_Pointer; }

private:
	void Assign(T * ptr)
	{
		if (ptr) ptr->IncRef();
		T * old = m_Pointer;
		m_Pointer = ptr;
		if (old) old->DecRef();
	}

	T * m_Pointer;
};

//------------------------------------------------------------------------------