// HashTable.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_CONTAINER_HASHMAP_H
#define FOUNDATION_CONTAINER_HASHMAP_H

// Include
//------------------------------------------------------------------------------
#include "Foundation/Container/HashTable.h"
#include "Foundation/Function/Hash.h"
#include "Foundation/Function/Compare.h"


template<
	class Key,
	class Data,
	class HashFunction = FNVHasher<Key>,
	class EqualKey = Equals<Key>,
	class Allocator = DefaultAllocator>
class HashMap
{
public:
	typedef KeyValuePair<Key, Data> ValueType;
	typedef HashTable<ValueType, Key, HashFunction, SelectKey<ValueType>, EqualKey, Allocator> Table;

	typedef typename Table::Iter      Iter;
	typedef typename Table::ConstIter ConstIter;

	explicit HashMap(SIZET bucketCount = Table::DEFAULT_BUCKET_COUNT, const Allocator & allocator = Allocator());
	explicit HashMap(const Allocator & allocator);
	HashMap(std::initializer_list<ValueType> initList);
	HashMap(const HashMap& rSource) = default;
	HashMap(HashMap && rOther) = default;

	~HashMap() = default;

	HashMap& operator=(const HashMap& rSource) = default;
	HashMap& operator=(HashMap&& rOther) = default;

	FORCE_INLINE Iter      Begin() { return m_Table.Begin(); }
	FORCE_INLINE ConstIter Begin() const { return m_Table.Begin(); }
	FORCE_INLINE Iter      End() { return m_Table.End(); }
	FORCE_INLINE ConstIter End() const { return m_Table.End(); }
	FORCE_INLINE Iter      Last() { return m_Table.Last(); }
	FORCE_INLINE ConstIter Last() const { return m_Table.Last(); }

	FORCE_INLINE Data & operator[](const Key& key) { return m_Table.FindOrInsert(ValueType(key, Data())).Second(); }

	FORCE_INLINE Iter             Find(const Key& key) { return m_Table.Find(key); }
	FORCE_INLINE ConstIter        Find(const Key& key) const { return m_Table.Find(key); }
	FORCE_INLINE Pair<Iter, bool> Insert(const ValueType& data) { return m_Table.InsertUnique(data); }
	FORCE_INLINE Pair<Iter, bool> Insert(ValueType&& data) { return m_Table.InsertUnique(std::forward<ValueType>(data)); }
	FORCE_INLINE SIZET			  Erase(const Key& key) { return m_Table.Erase(key); }

	FORCE_INLINE void Shrink() { m_Table.Shrink(); }
	FORCE_INLINE void Clear() { m_Table.Clear(); }

private:
	Table m_Table;
};


// Constructor
//------------------------------------------------------------------------------
template<class Key, class Data, class HashFunction, class EqualKey, class Allocator>
HashMap<Key, Data, HashFunction, EqualKey, Allocator>::HashMap(const Allocator & allocator)
	: m_Table(Table(DEFAULT_BUCKET_COUNT, HashFunction(), EqualKey(), allocator))
{
}

template<class Key, class Data, class HashFunction, class EqualKey, class Allocator>
HashMap<Key, Data, HashFunction, EqualKey, Allocator>::HashMap(SIZET bucketCount, const Allocator & allocator)
	: m_Table(Table(bucketCount, HashFunction(), EqualKey(), allocator))
{
}

template<class Key, class Data, class HashFunction, class EqualKey, class Allocator>
HashMap<Key, Data, HashFunction, EqualKey, Allocator>::HashMap(std::initializer_list<ValueType> initList)
	: m_Table(Table(Table::DEFAULT_BUCKET_COUNT, HashFunction(), EqualKey()))
{
	for (auto&& item : initList) { Insert(std::move(item)); }
}


//------------------------------------------------------------------------------
#endif // FOUNDATION_CONTAINER_HASHMAP_H