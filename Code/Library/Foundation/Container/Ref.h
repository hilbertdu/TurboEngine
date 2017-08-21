// Ref.h
//------------------------------------------------------------------------------
#pragma once

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"

// Ref class
//------------------------------------------------------------------------------
class Ref
{
public:
	explicit Ref(): m_RefCount(1) {};

	FORCE_INLINE int32 GetRefCount() const { return m_RefCount; };
	FORCE_INLINE int32 IncRefCount() { return ++m_RefCount; };
	FORCE_INLINE int32 DecRefCount() { return --m_RefCount; };

private:
	volatile int32 m_RefCount;
};


// RefPointerHolder class
//------------------------------------------------------------------------------
template<class T>
class RefPointerHolder: public Ref
{
public:
	explicit RefPointerHolder(): m_Pointer(nullptr), m_RefPointerCount(0) {};
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