// Singleton.h
//------------------------------------------------------------------------------
#pragma once
#ifndef CORE_CONTAINERS_SINGLETON_H
#define CORE_CONTAINERS_SINGLETON_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Env/Assert.h"

// Singleton
//------------------------------------------------------------------------------
template<class T>
class Singleton
{
public:
	static inline void Initialize();
	static inline T &  Instance();
	static inline bool IsValid() { return (s_Instance != nullptr); }

protected:
	Singleton();
	~Singleton();

private:
	static T * s_Instance;
};

// Static
//------------------------------------------------------------------------------
template<class T>
T * Singleton<T>::s_Instance = nullptr;

// CONSTRUCTOR
//------------------------------------------------------------------------------
template<class T>
Singleton<T>::Singleton()
{
	ASSERT(s_Instance == nullptr);
	s_Instance = static_cast<T *>(this);
}

// DESTRUCTOR
//------------------------------------------------------------------------------
template<class T>
Singleton<T>::~Singleton()
{
	ASSERT(s_Instance == this);
	s_Instance = nullptr;
}

// Initialize
//------------------------------------------------------------------------------
template<class T>
void Singleton<T>::Initialize()
{
	ASSERT(s_Instance == nullptr);
	s_Instance = TNEW(T);
}

// Instance
//------------------------------------------------------------------------------
template<class T>
T & Singleton<T>::Instance()
{
	ASSERT(s_Instance);
PRAGMA_DISABLE_PUSH_MSVC(6011) // static analysis generates a C6011: Dereferencing NULL pointer 's_Instance'
	return *s_Instance;
PRAGMA_DISABLE_POP_MSVC
}

//------------------------------------------------------------------------------
#endif // CORE_CONTAINERS_SINGLETON_H
