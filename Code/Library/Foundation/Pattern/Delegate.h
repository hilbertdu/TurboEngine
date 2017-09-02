// Delegate.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_PATTERN_DELEGATE_H
#define FOUNDATION_PATTERN_DELEGATE_H

// Include
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/Memory/Allocator.h"
#include "Foundation/Memory/Mem.h"
#include "Foundation/Container/AnyClass.h"


// Macros
//------------------------------------------------------------------------------
#define DELEGATE_DEFAULT_SIZE 16


// DelegateStorage
//------------------------------------------------------------------------------
template<int32 SIZE>
class DelegateStorage
{
public:
	DelegateStorage()
		: m_Mem(nullptr)
		, m_Size(0)
	{}

	DelegateStorage(const DelegateStorage & other) : m_Mem(nullptr), m_Size(0) { Assign(other); }
	DelegateStorage(DelegateStorage&& rOther) : m_Mem(nullptr), m_Size(0) { Assign(std::move(rOther)); }

	DelegateStorage & operator = (const DelegateStorage & other) { Assign(other); return *this; }
	DelegateStorage & operator = (DelegateStorage && other) { Assign(std::move(other)); return *this; }

	~DelegateStorage() { if (m_Mem) m_Allocator.Free(m_Mem); }
	
	void * SetStorage(SIZET size) 
	{
		if (m_Mem)
		{
			m_Allocator.Free(m_Mem);
		}

		if (size > 0)
		{
			m_Mem = m_Allocator.AllocateT<char>(size);
		}
		else
		{
			m_Mem = nullptr;
		}
		m_Size = size;

		return m_Mem;
	}
	
	inline void * GetStorage() const { return m_Mem; }
	inline bool IsStackMem() const { return m_Allocator.IsInStack(m_Mem); }

private:
	void Assign(const DelegateStorage & other)
	{
		SetStorage(0);
		if (other.m_Size > 0)
		{
			// copy
			m_Size = other.m_Size;
			m_Mem = m_Allocator.Allocate(m_Size);
			MemCopy(m_Mem, other.m_Mem, m_Size);
		}
	}

	void Assign(DelegateStorage && other)
	{
		SetStorage(0);
		if (other.m_Size > 0 && other.IsStackMem())
		{
			// stack move
			m_Size = other.m_Size;
			m_Mem = m_Allocator.Allocate(m_Size);
			MemCopy(m_Mem, other.m_Mem, m_Size);
		}
		else
		{
			// heap move
			m_Size = other.m_Size;
			m_Mem = other.m_Mem;
			other.m_Mem = nullptr;
		}
	}

	StackAllocator<SIZE> m_Allocator;
	void * m_Mem;
	SIZET  m_Size;
};


// Delegate
//------------------------------------------------------------------------------
template<typename T>
class Delegate;

template<typename T, typename... Args>
class Delegate<T(Args...)>
{
public:
	class IFunction
	{
	public:
		virtual ~IFunction() {};
		virtual void Destroy() = 0;
		virtual T Invoke(Args... args) const = 0;
#pragma push_macro("new")
#undef new
		static void * operator new(SIZET size, DelegateStorage<DELEGATE_DEFAULT_SIZE>* storage)
		{
			return storage->SetStorage(size);
		}
		static void operator delete(void * mem, DelegateStorage<DELEGATE_DEFAULT_SIZE>* storage) {}
	};
#pragma pop_macro("new")

	class StaticFunction : public IFunction
	{
	public:
		typedef T(*StaticFunctionPtr)(Args...);
		StaticFunction(const StaticFunctionPtr function)
			: m_Function(function)
		{}
		virtual ~StaticFunction() {};
		virtual void Destroy() { this->~StaticFunction(); };

		virtual T Invoke(Args... args) const { return m_Function(std::forward<Args>(args)...); }
	private:
		StaticFunctionPtr m_Function;
	};

	template<class C>
	class MethodFunction : public IFunction
	{
	public:
		typedef C* InstancePtr;
		typedef T(C::*MemberFunctionPtr)(Args...);
		MethodFunction(InstancePtr instance, MemberFunctionPtr method)
			: m_Method(method)
		{}
		virtual ~MethodFunction() {};
		virtual void Destroy() { this->~MethodFunction(); };

		virtual T Invoke(Args... args) const { return (m_Instance->*m_Method)(std::forward<Args>(args)...); }
		T Invoke(C * inst, Args... args) const { return (inst->*m_Method)(std::forward<Args>(args)...); }
	private:
		InstancePtr			m_Instance;
		MemberFunctionPtr	m_Method;
	};

	template<class Functor>
	class LambdaFunction : public IFunction
	{
	public:
		LambdaFunction(const Functor & functor)
			: m_Functor(functor)
		{}
		LambdaFunction(Functor && functor)
			: m_Functor(std::forward<Functor>(functor))
		{}
		virtual ~LambdaFunction() {};
		virtual void Destroy() { this->~LambdaFunction(); };

		virtual T Invoke(Args... args) const { return m_Functor(std::forward<Args>(args)...); }
	private:
		Functor m_Functor;
	};


	Delegate() {};
	Delegate(typename StaticFunction::StaticFunctionPtr function)
	{
		INPLACE_NEW(&m_Storage) StaticFunction(function);
	}
	template<class C>
	Delegate(C* c, T(C::* method)(Args...))
	{
		INPLACE_NEW(&m_Storage) MethodFunction<C>(c, method);
	}
	template<class Functor>
	Delegate(Functor&& function)
	{
		INPLACE_NEW(&m_Storage) LambdaFunction<Functor>(std::forward<Functor>(function));
	}
	Delegate(const Delegate& d): m_Storage(d.m_Storage) {}
	~Delegate() { Unbind(); }

	Delegate & operator = (const Delegate & other) { m_Storage = other.m_Storage; return *this; }
	Delegate & operator = (Delegate && rOther) { m_Storage = std::move(rOther.m_Storage); return *this; }

	void BindFunction(typename StaticFunction::StaticFunctionPtr function)
	{
		Unbind();
		INPLACE_NEW(&m_Storage) StaticFunction(function);
	}

	template<class C>
	void BindMethod(C* c, T(C::* method)(Args...))
	{
		Unbind();
		INPLACE_NEW(&m_Storage) MethodFunction<C>(c, method);
	}

	template<class Functor>
	void BindLambda(Functor&& function)
	{
		Unbind();
		INPLACE_NEW(&m_Storage) LambdaFunction<Functor>(std::forward<Functor>(function));
	}

	void Unbind()
	{
		if (IsValid())
		{
			reinterpret_cast<IFunction*>(m_Storage.GetStorage())->Destroy();
			m_Storage.SetStorage(0);
		}
	}

	bool IsValid() const
	{
		return m_Storage.GetStorage() != nullptr;
	}

	T Invoke(Args... args) const
	{
		ASSERT(IsValid());
		return reinterpret_cast<IFunction*>(m_Storage.GetStorage())->Invoke(std::forward<Args>(args)...);
	}

	template<class C>
	T Invoke(C * c, Args... args) const 
	{
		ASSERT(IsValid());
		return reinterpret_cast<MethodFunction<C>*>(m_Storage.GetStorage())->Invoke(c, std::forward<Args>(args)...);
	}

private:
	DelegateStorage<DELEGATE_DEFAULT_SIZE> m_Storage;
};


// AnyDelegate
//------------------------------------------------------------------------------
class AnyDelegate : public AnyClass
{
public:
	AnyDelegate() : AnyClass() {}

	template<class TReturn, class... TArgs>
	AnyDelegate(const Delegate<TReturn(TArgs...)> & d)
		: AnyClass(d)
	{}

	template<class TReturn, class... TArgs>
	AnyDelegate(Delegate<TReturn(TArgs...)>&& d)
		: AnyClass(std::move(d))
	{}

	template<class TReturn, class... TArgs>
	AnyDelegate(TReturn(*function)(TArgs...))
		: AnyClass(Delegate<TReturn(TArgs...)>(function))
	{}

	template<class C, class TReturn, class... TArgs>
	AnyDelegate(C * c, TReturn(C::* method)(TArgs...))
		: AnyClass(Delegate<TReturn(TArgs...)>(c, method))
	{}

	template<class TReturn, class... TArgs>
	AnyDelegate& operator=(Delegate<TReturn(TArgs...)>&& d)
	{
		AnyDelegate(std::forward<Delegate<TReturn(TArgs...)>>(d)).Swap(*this);
		return *this;
	}
};

//------------------------------------------------------------------------------
#endif // FOUNDATION_PATTERN_DELEGATE_H