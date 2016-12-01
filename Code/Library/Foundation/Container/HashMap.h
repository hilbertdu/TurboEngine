// HashTable.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_CONTAINER_HASHMAP_H
#define FOUNDATION_CONTAINER_HASHMAP_H

// Include
//------------------------------------------------------------------------------
#include "HashTable.h"
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
	static const size_t DEFAULT_BUCKET_COUNT = 37;

	typedef KeyValuePair<Key, Data> ValueType;
	typedef HashTable<ValueType, Key, HashFunction, SelectKey<ValueType>, EqualKey, Allocator> Table;

	typedef typename Table::Iterator      Iterator;
	typedef typename Table::ConstIterator ConstIterator;

	explicit HashMap(size_t bucketCount = DEFAULT_BUCKET_COUNT);
	HashMap(std::initializer_list<ValueType> initList);
	HashMap(const HashMap& rSource) = default;

	~HashMap() = default;

	HashMap& operator=(const HashMap& rSource) = default;

	FORCE_INLINE Iterator      Begin() { return m_Table.Begin(); }
	FORCE_INLINE Iterator      End() { return m_Table.End(); }
	FORCE_INLINE ConstIterator Begin() const { return m_Table.Begin(); }
	FORCE_INLINE ConstIterator End() const { return m_Table.End(); }

	FORCE_INLINE Data & operator[](const Key& key) { return m_Table.FindOrInsert(ValueType(key, Data())).Second(); }

	FORCE_INLINE Iterator             Find(const Key& key) { return m_Table.Find(key); }
	FORCE_INLINE Pair<Iterator, bool> Insert(const ValueType& data) { return m_Table.InsertUnique(data); }
	FORCE_INLINE size_t               Erase(const Key& key) { return m_Table.Erase(key); }

	FORCE_INLINE void Shrink() { m_Table.Shrink(); };

private:
	Table m_Table;
};


// Constructor
//------------------------------------------------------------------------------
template<class Key, class Data, class HashFunction, class EqualKey, class Allocator>
HashMap<Key, Data, HashFunction, EqualKey, Allocator>::HashMap(size_t bucketCount)
	: m_Table(Table(bucketCount, HashFunction(), EqualKey()))
{
}

template<class Key, class Data, class HashFunction, class EqualKey, class Allocator>
HashMap<Key, Data, HashFunction, EqualKey, Allocator>::HashMap(std::initializer_list<ValueType> initList)
	: m_Table(Table(DEFAULT_BUCKET_COUNT, HashFunction(), EqualKey()))
{
	for (std::initializer_list<ValueType>::const_iterator iter = initList.begin(); iter != initList.end(); ++iter)
	{
		Insert((*iter));
	}
}

#endif // FOUNDATION_CONTAINER_HASHMAP_H