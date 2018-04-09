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
class HashMap : public HashTable<KeyValuePair<Key, Data>, Key, HashFunction, SelectKey<KeyValuePair<Key, Data>>, EqualKey, Allocator>
{
public:
	typedef KeyValuePair<Key, Data> ValueType;
	typedef HashTable<ValueType, Key, HashFunction, SelectKey<ValueType>, EqualKey, Allocator> Table;

	explicit HashMap(SIZET bucketCount = Table::DEFAULT_BUCKET_COUNT, const Allocator & allocator = Allocator());
	explicit HashMap(const Allocator & allocator);
	HashMap(std::initializer_list<ValueType> initList);

	FORCE_INLINE Data & operator[](const Key& key) { return FindOrInsert(ValueType(key, Data())).Second(); }

	FORCE_INLINE Pair<Iter, bool> Insert(const ValueType& data) { return InsertUnique(data); }
	FORCE_INLINE Pair<Iter, bool> Insert(ValueType&& data) { return InsertUnique(std::forward<ValueType>(data)); }
};


// Constructor
//------------------------------------------------------------------------------
template<class Key, class Data, class HashFunction, class EqualKey, class Allocator>
HashMap<Key, Data, HashFunction, EqualKey, Allocator>::HashMap(const Allocator & allocator)
	: Table(DEFAULT_BUCKET_COUNT, HashFunction(), EqualKey(), allocator)
{
}

template<class Key, class Data, class HashFunction, class EqualKey, class Allocator>
HashMap<Key, Data, HashFunction, EqualKey, Allocator>::HashMap(SIZET bucketCount, const Allocator & allocator)
	: Table(bucketCount, HashFunction(), EqualKey(), allocator)
{
}

template<class Key, class Data, class HashFunction, class EqualKey, class Allocator>
HashMap<Key, Data, HashFunction, EqualKey, Allocator>::HashMap(std::initializer_list<ValueType> initList)
	: Table(Table::DEFAULT_BUCKET_COUNT, HashFunction(), EqualKey())
{
	for (auto&& item : initList) { Insert(std::move(item)); }
}


//------------------------------------------------------------------------------
#endif // FOUNDATION_CONTAINER_HASHMAP_H