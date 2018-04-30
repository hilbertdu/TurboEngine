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
	union Storage
	{
		void *	m_Heap;
		char	m_Stack[SIZE];
	};

	DelegateStorage() : m_Size(0) {}
	DelegateStorage(const DelegateStorage & other) : m_Size(0) { Assign(other); }
	DelegateStorage(DelegateStorage&& other) : m_Size(0) { Assign(std::move(other)); }

	DelegateStorage & operator = (const DelegateStorage & other) { Assign(other); return *this; }
	DelegateStorage & operator = (DelegateStorage && other) { Assign(std::move(other)); return *this; }

	~DelegateStorage() { Free(); }

	inline void * GetStorage() const { return m_Size == 0 ? nullptr : (m_Size > SIZE ? m_Storage.m_Heap : (void *)(&m_Storage.m_Stack[0])); }
	inline bool   IsStackMem() const { return m_Size <= SIZE; }

	void * SetStorage(SIZET size)
	{
		Free();
		if (size > SIZE)
		{
			m_Size = (int32)size;
			m_Storage.m_Heap = ALLOC(size);
			return m_Storage.m_Heap;
		}
		m_Size = (int32)size;
		return &(m_Storage.m_Stack[0]);
	}

private:
	inline void Assign(const DelegateStorage & other)
	{
		Free();
		void * mem = SetStorage(other.m_Size);
		MemCopy(mem, other.GetStorage(), m_Size);
	}

	inline void Assign(DelegateStorage && other)
	{
		Free();
		if (!other.IsStackMem())
		{
			m_Storage.m_Heap = other.m_Storage.m_Heap;
			m_Size = other.m_Size;
			other.m_Size = 0;
		}
		else
		{
			MemCopy(m_Storage.m_Stack, other.GetStorage(), other.m_Size);
			m_Size = other.m_Size;
			other.m_Size = 0;
		}
	}

	inline void Free() { if (m_Size > SIZE) FREE(m_Storage.m_Heap); }

	Storage	m_Storage;
	uint32	m_Size;
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
			: m_Instance(instance)
			, m_Method(method)
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
	Delegate(Delegate&& d) : m_Storage(std::move(d.m_Storage)) {}
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
	/*
	T operator()(Args... args) const
	{
		ASSERT(IsValid());
		return reinterpret_cast<IFunction*>(m_Storage.GetStorage())->Invoke(std::forward<Args>(args)...);
	}*/

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
	template<class TReturn, class... TArgs> AnyDelegate(const Delegate<TReturn(TArgs...)> & d) : AnyClass(d) {}
	template<class TReturn, class... TArgs> AnyDelegate(Delegate<TReturn(TArgs...)>&& d) : AnyClass(std::move(d)) {}
	template<class TReturn, class... TArgs> AnyDelegate(TReturn(*function)(TArgs...)) : AnyClass(Delegate<TReturn(TArgs...)>(function)) {}
	template<class C, class TReturn, class... TArgs> AnyDelegate(C * c, TReturn(C::* method)(TArgs...)) : AnyClass(Delegate<TReturn(TArgs...)>(c, method)) {}
};

//------------------------------------------------------------------------------
#endif // FOUNDATION_PATTERN_DELEGATE_H
