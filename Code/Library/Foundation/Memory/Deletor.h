// Deletor.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_MEMORY_DELETOR_H
#define FOUNDATION_MEMORY_DELETOR_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Memory/Mem.h"

template<class AForm, class Category>
class PoolSizeAllocator;


// DefaultDeletor
//------------------------------------------------------------------------------
template<class T>
class NewDeletor
{
public:
	void Delete(T * ptr) { TDELETE(ptr); }
};

template<class T>
class ArrayDeletor
{
public:
	void Delete(T * ptr) { TDELETE_ARRAY(ptr); }
};

class AllocDeletor
{
public:
	inline void Delete(void * ptr) { FREE(ptr); }
};

template<class Form, class Category = CONSTSTR("Default")>
class PoolDeletor
{
public:
	PoolDeletor(PoolSizeAllocator<Form, Category>* allocator) { m_Allocator = allocator; }

	inline void Delete(void * ptr) { m_Allocator->Free(ptr); }

private:
	PoolSizeAllocator<Form, Category>* m_Allocator;
};


//------------------------------------------------------------------------------
#endif // FOUNDATION_MEMORY_DELETOR_H