// Generator.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_MISC_GENERATOR_H
#define FOUNDATION_MISC_GENERATOR_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Container/Array.h"

template<class T, uint32 StackSize = 128>
class IdGenerator
{
public:
	IdGenerator();

	T	 Pop();
	void Push(T id);

private:
	T m_Id;
	T m_MaxId;
	StackArray<T, StackSize> m_Stack;
};

//------------------------------------------------------------------------------
#endif // FOUNDATION_MISC_GENERATOR_H