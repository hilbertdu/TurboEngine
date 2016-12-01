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
	HashTableIterator& operator=(const HashTableIterator& rOther) = default;

	HashTableIterator(const HashTableIterator<Table, !Const>& rOther);
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
	size_t m_BucketIndex;
	size_t m_ElementIndex;

	explicit HashTableIterator(Table* pTable, size_t bucketIndex, size_t elementIndex);

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
	static const size_t DEFAULT_BUCKET_COUNT = 37;

	typedef HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator> MyType;

	typedef Key				KeyType;
	typedef Value			ValueType;
	typedef HashFunction	HasherType;
	typedef ExtractKey      ExtractKeyType;
	typedef EqualKey		KeyEqualType;
	typedef Allocator		AllocatorType;

	typedef HashTableIterator<MyType, false> Iterator;
	typedef HashTableIterator<MyType, true>  ConstIterator;

	explicit HashTable(size_t bucketCount, const HashFunction& rHasher, const EqualKey& rKeyEquals, const ExtractKey& rExtractKey, const Allocator& rAllocator = Allocator());
	explicit HashTable(size_t bucketCount, const HashFunction& rHasher, const EqualKey& rKeyEquals, const Allocator& rAllocator = Allocator());

	HashTable(const HashTable& rSource);
	template<class OtherAllocator>
	HashTable(const HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, OtherAllocator>& rSource);
	~HashTable();

	HashTable& operator=(const HashTable& rSource);
	template<class OtherAllocator>
	HashTable& operator=(const HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, OtherAllocator>& rSource);

	size_t GetSize() const;
	bool   IsEmpty() const;
	void   Clear();
	void   Shrink();
	void   Swap(HashTable& rTable);

	Iterator      Begin();
	Iterator      End();
	ConstIterator Begin() const;
	ConstIterator End() const;

	Iterator      Find(const Key& rKey);
	ConstIterator Find(const Key& rKey) const;

	Pair<Iterator, bool> InsertUnique(const Value& rValue);
	bool                 InsertUnique(Iterator& rIterator, const Value& rValue);
	Iterator             InsertEqual(const Value& rValue);

	Value & FindOrInsert(const Value& rValue);

	size_t   Erase(const Key& rKey);
	Iterator Erase(ConstIterator& iterator);

protected:
	typedef Array<Value, Allocator> Bucket;

	Bucket* m_Buckets;
	size_t  m_BucketCount;
	size_t  m_Size;

	HashFunction m_Hasher;
	EqualKey     m_KeyEquals;
	ExtractKey   m_ExtractKey;
	Allocator    m_Allocator;

private:
	void AllocateBuckets();

	template<class OtherAllocator>
	void CopyConstruct(const HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, OtherAllocator>& rSource);
	void Finalize();

	friend class HashTableIterator<MyType, false>;
	friend class HashTableIterator<MyType, true>;
};


#include "Foundation/Container/HashTable.inl"

#endif // FOUNDATION_CONTAINER_HASHTABLE_H