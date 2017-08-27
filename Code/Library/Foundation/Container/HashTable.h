// HashTable.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_CONTAINER_HASHTABLE_H
#define FOUNDATION_CONTAINER_HASHTABLE_H

// Include
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/Env/Assert.h"
#include "Foundation/Platform/Misc.h"
#include "Foundation/Memory/Allocator.h"
#include "Foundation/Memory/Mem.h"
#include "Foundation/Container/Array.h"
#include "Foundation/Container/Pair.h"
#include <type_traits>


// Forward declaration
//------------------------------------------------------------------------------
template<
	class Value,
	class Key,
	class HashFunction,
	class ExtractKey,
	class EqualKey,
	class Allocator>
class HashTable;

// HashTableIterator
//------------------------------------------------------------------------------
template<class Table, bool Const = true>
class HashTableIterator
{
public:
	typedef typename Table::ValueType        ValueType;
	typedef typename Table::KeyType          KeyType;
	typedef typename Table::ValueType        ValueType;
	typedef typename Table::HasherType       HasherType;
	typedef typename Table::ExtractKeyType   ExtractKeyType;
	typedef typename Table::KeyEqualType     KeyEqualType;
	typedef typename Table::AllocatorType    AllocatorType;

	explicit HashTableIterator() = default;
	HashTableIterator(const HashTableIterator& rOther) = default;
	HashTableIterator(const HashTableIterator<Table, !Const>& rOther);

	HashTableIterator& operator=(const HashTableIterator& rOther) = default;
	HashTableIterator& operator=(const HashTableIterator<Table, !Const>& rOther);

	const ValueType& operator*() const;
	const ValueType* operator->() const;

	template<class = std::enable_if<!Const>::type>
	ValueType& operator*();

	template<class = std::enable_if<!Const>::type>
	ValueType* operator->();

	HashTableIterator& operator++();
	HashTableIterator  operator++(int);
	HashTableIterator& operator--();
	HashTableIterator  operator--(int);

	bool operator==(const HashTableIterator& rOther) const;
	bool operator!=(const HashTableIterator& rOther) const;
	bool operator<(const HashTableIterator& rOther) const;
	bool operator>(const HashTableIterator& rOther) const;
	bool operator<=(const HashTableIterator& rOther) const;
	bool operator>=(const HashTableIterator& rOther) const;

protected:
	Table* m_Table;
	SIZET m_BucketIndex;
	SIZET m_ElementIndex;

	explicit HashTableIterator(Table* pTable, SIZET bucketIndex, SIZET elementIndex);

	friend class HashTable<ValueType, KeyType, HasherType, ExtractKeyType, KeyEqualType, AllocatorType>;
	friend class HashTableIterator<Table, !Const>;
};


// HashTable
//------------------------------------------------------------------------------
template<
	class Value,
	class Key,
	class HashFunction,
	class ExtractKey,
	class EqualKey,
	class Allocator = DefaultAllocator>
class HashTable
{
public:
	static constexpr SIZET DEFAULT_BUCKET_COUNT = 16;
	static constexpr float DEFAULT_LOAD_FACTOR = 1.0;

	typedef HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator> MyType;

	typedef Key				KeyType;
	typedef Value			ValueType;
	typedef HashFunction	HasherType;
	typedef ExtractKey      ExtractKeyType;
	typedef EqualKey		KeyEqualType;
	typedef Allocator		AllocatorType;

	typedef HashTableIterator<MyType, false> Iter;
	typedef HashTableIterator<MyType, true>  ConstIter;

	explicit HashTable(SIZET bucketCount, const HashFunction& rHasher, const EqualKey& rKeyEquals, const ExtractKey& rExtractKey, const Allocator& rAllocator = Allocator());
	explicit HashTable(SIZET bucketCount, const HashFunction& rHasher, const EqualKey& rKeyEquals, const Allocator& rAllocator = Allocator());

	HashTable(const HashTable& rSource);
	template<class OtherAllocator>
	HashTable(const HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, OtherAllocator>& rSource);
	HashTable(HashTable && rOther);

	~HashTable();

	HashTable& operator=(const HashTable& rSource);
	template<class OtherAllocator>
	HashTable& operator=(const HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, OtherAllocator>& rSource);
	HashTable& operator=(HashTable&& rOther);
	
	SIZET GetSize() const;
	bool  IsEmpty() const;
	void  Clear();
	void  Shrink();
	void  Swap(HashTable& rTable);
	void  SetPossibleMaxSize();

	Iter      Begin();
	ConstIter Begin() const;
	Iter      End();
	ConstIter End() const;
	Iter      Last();
	ConstIter Last() const;

	Iter      Find(const Key& rKey);
	ConstIter Find(const Key& rKey) const;

	Pair<Iter, bool> InsertUnique(const Value& rValue);
	Pair<Iter, bool> InsertUnique(Value&& rValue);
	bool             InsertUnique(Iter& rIterator, const Value& rValue);
	bool             InsertUnique(Iter& rIterator, Value&& rValue);
	Iter             InsertEqual(const Value& rValue);

	Value & FindOrInsert(const Value& rValue);

	SIZET	Erase(const Key& rKey);
	Iter	Erase(ConstIter& iterator);

protected:
	typedef Array<Value, Allocator> Bucket;	

	Allocator		m_Allocator;
	Array<Bucket>	m_Buckets;
	SIZET			m_Size;

	HashFunction m_Hasher;
	EqualKey     m_KeyEquals;
	ExtractKey   m_ExtractKey;

private:
	void ReHash(SIZET size);

	template<class ValueT>
	bool _InsertUnique(Iter& rIterator, ValueT&& rValue);

	template<class OtherAllocator>
	void CopyConstruct(const HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, OtherAllocator>& rSource);

	friend class HashTableIterator<MyType, false>;
	friend class HashTableIterator<MyType, true>;
};


#include "Foundation/Container/HashTable.inl"

//------------------------------------------------------------------------------
#endif // FOUNDATION_CONTAINER_HASHTABLE_H