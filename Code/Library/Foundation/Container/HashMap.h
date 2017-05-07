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
	class HashFunction = Hash<Key>,
	class EqualKey = Equals<Key>,
	class Allocator = DefaultAllocator>
class HashMap
{
public:
	static const SIZET DEFAULT_BUCKET_COUNT = 37;

	typedef KeyValuePair<Key, Data> ValueType;
	typedef HashTable<ValueType, Key, HashFunction, SelectKey<ValueType>, EqualKey, Allocator> Table;

	typedef typename Table::Iterator      Iterator;
	typedef typename Table::ConstIterator ConstIterator;

	explicit HashMap(SIZET bucketCount = DEFAULT_BUCKET_COUNT, const Allocator & allocator = Allocator());
	explicit HashMap(const Allocator & allocator);
	HashMap(const std::initializer_list<ValueType> & initList);
	HashMap(const HashMap& rSource) = default;
	HashMap(HashMap && rOther) = default;

	~HashMap() = default;

	HashMap& operator=(const HashMap& rSource) = default;
	HashMap& operator=(HashMap&& rOther) = default;

	FORCE_INLINE Iterator      Begin() { return m_Table.Begin(); }
	FORCE_INLINE Iterator      End() { return m_Table.End(); }
	FORCE_INLINE ConstIterator Begin() const { return m_Table.Begin(); }
	FORCE_INLINE ConstIterator End() const { return m_Table.End(); }

	FORCE_INLINE Data & operator[](const Key& key) { return m_Table.FindOrInsert(ValueType(key, Data())).Second(); }

	FORCE_INLINE Iterator             Find(const Key& key) { return m_Table.Find(key); }
	FORCE_INLINE Pair<Iterator, bool> Insert(const ValueType& data) { return m_Table.InsertUnique(data); }
	FORCE_INLINE SIZET               Erase(const Key& key) { return m_Table.Erase(key); }

	FORCE_INLINE void Shrink() { m_Table.Shrink(); };

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
HashMap<Key, Data, HashFunction, EqualKey, Allocator>::HashMap(const std::initializer_list<ValueType> & initList)
	: m_Table(Table(DEFAULT_BUCKET_COUNT, HashFunction(), EqualKey()))
{
	for (auto item : initList) { Insert(item); }
}


//------------------------------------------------------------------------------
#endif // FOUNDATION_CONTAINER_HASHMAP_H