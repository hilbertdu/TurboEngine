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

	inline int32 GetRefCount() const { return m_RefCount; };
	inline int32 IncRefCount() { return ++m_RefCount; };
	inline int32 DecRefCount() { return --m_RefCount; };

private:
	volatile int32 m_RefCount;
};


template<class T>
class RefPointHolder: public Ref
{
public:
	explicit RefPointHolder(): m_Pointer(nullptr), m_RefPointerCount(0) {};
	explicit RefPointHolder(T* pointer) : m_Pointer(pointer), m_RefPointerCount(1) {};

	inline T* GetPointer() { return m_Pointer; };
	inline void SetPointer(T* pointer) { m_Pointer = pointer; m_RefPointerCount = 1; };

	inline int32 GetPointerRefCount() { return m_RefPointerCount; };

	inline int32 IncPointerRefCount() { return ++m_RefPointerCount; };
	inline int32 DecPointerRefCount() { return --m_RefPointerCount; };

private:
	T* m_Pointer;
	volatile int32 m_RefPointerCount;
};