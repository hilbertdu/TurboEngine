// Iterators.h
// container iterators
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_ITERATORS_H
#define FOUNDATION_REFLECTION_ITERATORS_H


// Includes
//------------------------------------------------------------------------------
#include "Foundation/Container/Array.h"
#include "Foundation/Reflection/Container/Iterator.h"


namespace TReflection
{
	// ArrayReadIterator
	//------------------------------------------------------------------------------
	template <typename TYPE, typename ALLOCATOR>
	class ArrayReadIterator : public IReadIterator
	{
	public:
		typedef Array<TYPE, ALLOCATOR> Container;
		typedef typename Container::ConstIter Iterator;

		ArrayReadIterator() {}

		ArrayReadIterator(const Container* container) 
			: m_Container(container)
			, m_Iterator(container->Begin())
		{}
		
		virtual const void* GetKey() const
		{
			ASSERT(false);
			return 0;
		}

		virtual const void* GetValue() const
		{
			ASSERT(m_Iterator != m_Container->End());
			return &(*m_Iterator);
		}

		virtual uint32 GetCount() const
		{
			return m_Container->GetSize();
		}

		virtual void MoveNext()
		{
			ASSERT(IsValid());
			++m_Iterator;
		}
		
		virtual bool IsValid() const
		{
			return m_Iterator != m_Container->End();
		}

		virtual void ResetContainer(const void *container)
		{
			m_Container = (Container*)container;
			m_Iterator = m_Container->Begin();
		}

	private:
		Container* m_Container{ 0 };
		Iterator m_Iterator{ 0 };
	};

	// ArrayWriteIterator
	//------------------------------------------------------------------------------
	template <typename TYPE, typename ALLOCATOR>
	class ArrayWriteIterator : public IWriteIterator
	{
	public:
		typedef Array<TYPE, ALLOCATOR> Container;

		ArrayWriteIterator() {}

		ArrayWriteIterator(Container* container)
			: m_Container(container)
		{}

		virtual void Add(void* object)
		{
			m_Container->Append(*(TYPE*)object);
		}

		virtual void Add(void*, void*)
		{
			ASSERT(false);
		}

		virtual void* AddEmpty()
		{
			m_Container->Append(TYPE());
			return m_Container->Last();
		}

		virtual void* AddEmpty(void*)
		{
			ASSERT(false);
			return 0;
		}

		virtual void ResetContainer(const void *container)
		{
			m_Container = (Container*)container;
			m_Container->Clear();
		}

	private:
		Container* m_Container{ 0 };
	};


	// LinkedListReadIterator
	//------------------------------------------------------------------------------
	template <typename TYPE, typename ALLOCATOR>
	class LinkedListReadIterator : public IReadIterator
	{
		virtual const void* GetKey() const { return 0; }
		virtual const void* GetValue() const { return 0; }
		virtual uint32 GetCount() const { return 0; }
		virtual void MoveNext() {}
		virtual bool IsValid() const { return false; }
		virtual void ResetContainer(const void *container) {}
	};

	// LinkedListWriteIterator
	//------------------------------------------------------------------------------
	template <typename TYPE, typename ALLOCATOR>
	class LinkedListWriteIterator : public IWriteIterator
	{
		virtual void Add(void* object) {}
		virtual void Add(void* key, void* object) {}
		virtual void* AddEmpty() { return 0; }
		virtual void* AddEmpty(void* key) { return 0; }
		virtual void ResetContainer(const void *container) {}
	};

	// PairReadIterator
	//------------------------------------------------------------------------------
	template <typename TYPE1, typename TYPE2>
	class PairReadIterator : public IReadIterator
	{
		virtual const void* GetKey() const { return 0; }
		virtual const void* GetValue() const { return 0; }
		virtual uint32 GetCount() const { return 0; }
		virtual void MoveNext() {}
		virtual bool IsValid() const { return false; }
		virtual void ResetContainer(const void *container) {}
	};

	// PairWriteIterator
	//------------------------------------------------------------------------------
	template <typename TYPE1, typename TYPE2>
	class PairWriteIterator : public IWriteIterator
	{
		virtual void Add(void* object) {}
		virtual void Add(void* key, void* object) {}
		virtual void* AddEmpty() { return 0; }
		virtual void* AddEmpty(void* key) { return 0; }
		virtual void ResetContainer(const void *container) {}
	};

	// HashMapReadIterator
	//------------------------------------------------------------------------------
	template <typename KEY, typename DATA, typename HASHER, typename EQUAL, typename ALLOCATOR>
	class HashMapReadIterator : public IReadIterator
	{
		virtual const void* GetKey() const { return 0; }
		virtual const void* GetValue() const { return 0; }
		virtual uint32 GetCount() const { return 0; }
		virtual void MoveNext() {}
		virtual bool IsValid() const { return false; }
		virtual void ResetContainer(const void *container) {}
	};

	// HashMapWriteIterator
	//------------------------------------------------------------------------------
	template <typename KEY, typename DATA, typename HASHER, typename EQUAL, typename ALLOCATOR>
	class HashMapWriteIterator : public IWriteIterator
	{
		virtual void Add(void* object) {}
		virtual void Add(void* key, void* object) {}
		virtual void* AddEmpty() { return 0; }
		virtual void* AddEmpty(void* key) { return 0; }
		virtual void ResetContainer(const void *container) {}
	};

	// StrongPtrReadIterator
	//------------------------------------------------------------------------------
	template <typename TYPE, typename DELETOR>
	class StrongPtrReadIterator : public IReadIterator
	{
		virtual const void* GetKey() const { return 0; }
		virtual const void* GetValue() const { return 0; }
		virtual uint32 GetCount() const { return 0; }
		virtual void MoveNext() {}
		virtual bool IsValid() const { return false; }
		virtual void ResetContainer(const void *container) {}
	};

	// StrongPtrWriteIterator
	//------------------------------------------------------------------------------
	template <typename TYPE, typename DELETOR>
	class StrongPtrWriteIterator : public IWriteIterator
	{
		virtual void Add(void* object) {}
		virtual void Add(void* key, void* object) {}
		virtual void* AddEmpty() { return 0; }
		virtual void* AddEmpty(void* key) { return 0; }
		virtual void ResetContainer(const void *container) {}
	};

	// WeakPtrReadIterator
	//------------------------------------------------------------------------------
	template <typename TYPE, typename DELETOR>
	class WeakPtrReadIterator : public IReadIterator
	{
		virtual const void* GetKey() const { return 0; }
		virtual const void* GetValue() const { return 0; }
		virtual uint32 GetCount() const { return 0; }
		virtual void MoveNext() {}
		virtual bool IsValid() const { return false; }
		virtual void ResetContainer(const void *container) {}
	};

	// WeakPtrWriteIterator
	//------------------------------------------------------------------------------
	template <typename TYPE, typename DELETOR>
	class WeakPtrWriteIterator : public IWriteIterator
	{
		virtual void Add(void* object) {}
		virtual void Add(void* key, void* object) {}
		virtual void* AddEmpty() { return 0; }
		virtual void* AddEmpty(void* key) { return 0; }
		virtual void ResetContainer(const void *container) {}
	};
}

#endif // FOUNDATION_REFLECTION_ITERATORS_H
//------------------------------------------------------------------------------
