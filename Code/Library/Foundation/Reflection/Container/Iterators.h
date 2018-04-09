// Iterators.h
// container iterators
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_REFLECTION_ITERATORS_H
#define FOUNDATION_REFLECTION_ITERATORS_H


// Includes
//------------------------------------------------------------------------------
#include "Foundation/Container/Array.h"
#include "Foundation/Container/HashMap.h"
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

		virtual IReadIterator* Clone()
		{
			return TNEW(ArrayReadIterator());
		}

		virtual bool IsKeyPointer() const { ASSERT(false); return false; }
		virtual bool IsValuePointer() const { return std::is_pointer<TYPE>::value; }

	private:
		Container * m_Container{ 0 };
		Iterator	m_Iterator{ 0 };
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

		virtual IWriteIterator* Clone()
		{
			return TNEW(ArrayWriteIterator());
		}

		virtual bool IsKeyPointer() const { ASSERT(false); return false; }
		virtual bool IsValuePointer() const { return std::is_pointer<TYPE>::value; }

	private:
		Container* m_Container{ 0 };
	};


	// LinkedListReadIterator
	//------------------------------------------------------------------------------
	template <typename TYPE, typename ALLOCATOR>
	class LinkedListReadIterator : public IReadIterator
	{
	public:
		virtual const void* GetKey() const { return 0; }
		virtual const void* GetValue() const { return 0; }
		virtual uint32 GetCount() const { return 0; }
		virtual void MoveNext() {}
		virtual bool IsValid() const { return false; }
		virtual void ResetContainer(const void *container) {}
		virtual bool IsKeyPointer() const { return false; }
		virtual bool IsValuePointer() const { return false; }
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
		virtual bool IsKeyPointer() const { return false; }
		virtual bool IsValuePointer() const { return false; }
	};

	// PairReadIterator
	//------------------------------------------------------------------------------
	template <typename TYPE1, typename TYPE2>
	class PairReadIterator : public IReadIterator
	{
	public:
		virtual const void* GetKey() const { return 0; }
		virtual const void* GetValue() const { return 0; }
		virtual uint32 GetCount() const { return 0; }
		virtual void MoveNext() {}
		virtual bool IsValid() const { return false; }
		virtual void ResetContainer(const void *container) {}
		virtual bool IsKeyPointer() const { return false; }
		virtual bool IsValuePointer() const { return false; }
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
		virtual bool IsKeyPointer() const { return false; }
		virtual bool IsValuePointer() const { return false; }
	};

	// HashMapReadIterator
	//------------------------------------------------------------------------------
	template <typename KEY, typename DATA, typename HASHER, typename EQUAL, typename ALLOCATOR>
	class HashMapReadIterator : public IReadIterator
	{
	public:
		typedef HashMap<KEY, DATA, HASHER, EQUAL, ALLOCATOR> Container;
		typedef typename Container::ConstIter Iterator;

		HashMapReadIterator() {}

		HashMapReadIterator(Container* container)
			: m_Container(container)
			, m_Iterator(container->Begin())
		{}

		virtual const void* GetKey() const 
		{ 
			return &(m_Iterator->First());
		}

		virtual const void* GetValue() const
		{
			return &(m_Iterator->Second());
		}

		virtual uint32 GetCount() const
		{
			return m_Container->GetSize();
		}

		virtual void MoveNext()
		{
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

		virtual IReadIterator* Clone()
		{
			return TNEW(HashMapReadIterator());
		}

		virtual bool IsKeyPointer() const { return std::is_pointer<KEY>::value; }
		virtual bool IsValuePointer() const { return std::is_pointer<DATA>::value; }

	private:
		Container * m_Container{ 0 };
		Iterator	m_Iterator;
	};

	// HashMapWriteIterator
	//------------------------------------------------------------------------------
	template <typename KEY, typename DATA, typename HASHER, typename EQUAL, typename ALLOCATOR>
	class HashMapWriteIterator : public IWriteIterator
	{
	public:
		typedef HashMap<KEY, DATA, HASHER, EQUAL, ALLOCATOR> Container;
		typedef typename Container::Iter Iterator;

		HashMapWriteIterator() {}

		HashMapWriteIterator(Container* container)
			: m_Container(container)
		{}

		virtual void Add(void* object)
		{
			ASSERT(0);
		}

		virtual void Add(void* key, void* object)
		{
			m_Container->Insert(KeyValuePair<KEY, DATA>(*(KEY*)key, *(DATA*)object));
		}

		virtual void* AddEmpty()
		{
			ASSERT(0);
			return 0;
		}

		virtual void* AddEmpty(void* key)
		{
			Pair<Container::Iter, bool> result = m_Container->Insert(KeyValuePair<KEY, DATA>(*(KEY*)key, DATA()));
			return &(result.First()->Second());
		}

		virtual void ResetContainer(const void *container)
		{
			m_Container = (Container*)container;
		}

		virtual IWriteIterator* Clone()
		{
			return TNEW(HashMapWriteIterator());
		}

		virtual bool IsKeyPointer() const { return std::is_pointer<KEY>::value; }
		virtual bool IsValuePointer() const { return std::is_pointer<DATA>::value; }

	private:
		Container * m_Container{ 0 };
	};

	// RefReadIterator
	//------------------------------------------------------------------------------
	template <typename TYPE>
	class RefReadIterator : public IReadIterator
	{
	public:
		typedef Ref<TYPE> Container;

		RefReadIterator() {}

		RefReadIterator(const Container* contaner)
			: m_Container(contaner)
			, m_IsValid(true)
		{}

		virtual const void* GetKey() const
		{
			ASSERT(false);
			return 0;
		}

		virtual const void* GetValue() const
		{
			return &(m_Container->GetPointerRef());
		}

		virtual uint32 GetCount() const
		{
			return 1;
		}

		virtual void MoveNext()
		{
			ASSERT(IsValid());
			m_IsValid = false;
		}

		virtual bool IsValid() const
		{
			return m_IsValid;
		}

		virtual void ResetContainer(const void *container)
		{
			m_Container = (Container*)container;
			m_IsValid = true;
		}

		virtual IReadIterator* Clone()
		{
			return TNEW(RefReadIterator());
		}

		virtual bool IsKeyPointer() const { ASSERT(false); return false; }
		virtual bool IsValuePointer() const { return true; }

	private:
		Container* m_Container{ 0 };
		bool m_IsValid{ false };
	};

	// RefWriteIterator
	//------------------------------------------------------------------------------
	template <typename TYPE>
	class RefWriteIterator : public IWriteIterator
	{
	public:
		typedef Ref<TYPE> Container;

		RefWriteIterator() {}

		RefWriteIterator(const Container* contaner)
			: m_Container(contaner)
		{}

		virtual void Add(void* object)
		{
			*m_Container = (*(TYPE**)object);
		}

		virtual void Add(void* key, void* object)
		{
			ASSERT(false);
		}

		virtual void* AddEmpty()
		{
			ASSERT(false);
			return 0;
		}

		virtual void* AddEmpty(void* key)
		{
			ASSERT(false);
			return 0;
		}

		virtual void ResetContainer(const void *container)
		{
			m_Container = (Container*)container;
		}

		virtual IWriteIterator* Clone()
		{
			return TNEW(RefWriteIterator());
		}

		virtual bool IsKeyPointer() const { ASSERT(false); return false; }
		virtual bool IsValuePointer() const { return true; }

	private:
		Container* m_Container{ 0 };
	};
}

#endif // FOUNDATION_REFLECTION_ITERATORS_H
//------------------------------------------------------------------------------
