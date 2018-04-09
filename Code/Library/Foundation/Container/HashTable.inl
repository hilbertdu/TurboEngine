// HashTableIterator functions
//------------------------------------------------------------------------------

// Constructors
//------------------------------------------------------------------------------
template<class Table, bool Const>
HashTableIterator<Table, Const>::HashTableIterator(TablePtr pTable, SIZET bucketIndex, SIZET elementIndex)
	: m_Table(pTable)
	, m_BucketIndex(bucketIndex)
	, m_ElementIndex(elementIndex)
{
}

template<class Table, bool Const>
HashTableIterator<Table, Const>::HashTableIterator(const HashTableIterator<Table, !Const>& rOther)
{
	m_Table = rOther.m_Table;
	m_BucketIndex = rOther.m_BucketIndex;
	m_ElementIndex = rOther.m_ElementIndex;
}

template<class Table, bool Const>
HashTableIterator<Table, Const>& HashTableIterator<Table, Const>::operator=(const HashTableIterator<Table, !Const>& rOther)
{
	m_Table = rOther.m_Table;
	m_BucketIndex = rOther.m_BucketIndex;
	m_ElementIndex = rOther.m_ElementIndex;
	return *this;
}

// Operators
//------------------------------------------------------------------------------
template<class Table, bool Const>
typename const HashTableIterator<Table, Const>::ValueType& HashTableIterator<Table, Const>::operator*() const
{
	ASSERT(m_Table);
	return m_Table->m_Buckets[m_BucketIndex][m_ElementIndex];
}

template<class Table, bool Const>
typename const HashTableIterator<Table, Const>::ValueType* HashTableIterator<Table, Const>::operator->() const
{
	ASSERT(m_Table);
	return &m_Table->m_Buckets[m_BucketIndex][m_ElementIndex];
}

template<class Table, bool Const>
template<class>
typename HashTableIterator<Table, Const>::ValueType& HashTableIterator<Table, Const>::operator*()
{
	ASSERT(m_Table);
	return m_Table->m_Buckets[m_BucketIndex][m_ElementIndex];
}

template<class Table, bool Const>
template<class>
typename HashTableIterator<Table, Const>::ValueType* HashTableIterator<Table, Const>::operator->()
{
	ASSERT(m_Table);
	return &m_Table->m_Buckets[m_BucketIndex][m_ElementIndex];
}

template<class Table, bool Const>
HashTableIterator<Table, Const>& HashTableIterator<Table, Const>::operator++()
{
	ASSERT(m_Table);
	ASSERT(m_BucketIndex < m_Table->m_Buckets.GetSize());

	++m_ElementIndex;

	if (m_ElementIndex >= m_Table->m_Buckets[m_BucketIndex].GetSize())
	{
		m_ElementIndex = 0;

		SIZET bucketCount = m_Table->m_Buckets.GetSize();
		for (SIZET bucketIdx = m_BucketIndex + 1; bucketIdx < bucketCount; ++bucketIdx)
		{
			if (!m_Table->m_Buckets[bucketIdx].IsEmpty())
			{
				m_BucketIndex = bucketIdx;
				return *this;
			}
		}
		m_BucketIndex = bucketCount;	// End()
	}

	return *this;
}

template<class Table, bool Const>
HashTableIterator<Table, Const> HashTableIterator<Table, Const>::operator++(int)
{
	HashTableIterator iterator = *this;
	++(*this);
	return iterator;
}

template<class Table, bool Const>
HashTableIterator<Table, Const>& HashTableIterator<Table, Const>::operator--()
{
	ASSERT(m_Table);
	ASSERT(m_BucketIndex <= m_Table->m_Buckets.GetSize());

	if (m_ElementIndex > 0)
	{
		--m_ElementIndex;
		return *this;
	}

	typename Table::Bucket* pBuckets = m_Table->m_Buckets;
	ASSERT(pBuckets);

	SIZET bucketIdx = m_BucketIndex;
	while (bucketIdx != 0)
	{
		--bucketIdx;

		SIZET entryCount = pBuckets[bucketIdx].GetSize();
		if (entryCount != 0)
		{
			m_BucketIndex = bucketIdx;
			m_ElementIndex = entryCount - 1;
			return *this;
		}
	}

	ASSERT(false, "Attempted backward HashTable iteration past the start of the table");

	--m_Table;
	m_BucketIndex = 0;
	m_ElementIndex = 0;
	return *this;
}

template<class Table, bool Const>
HashTableIterator<Table, Const> HashTableIterator<Table, Const>::operator--(int)
{
	HashTableIterator iterator = *this;
	--(*this);
	return iterator;
}

template<class Table, bool Const>
bool HashTableIterator<Table, Const>::operator==(const HashTableIterator& rOther) const
{
	return (
		m_Table == rOther.m_Table && 
		m_BucketIndex == rOther.m_BucketIndex &&
		m_ElementIndex == rOther.m_ElementIndex
		);
}

template<class Table, bool Const>
bool HashTableIterator<Table, Const>::operator!=(const HashTableIterator& rOther) const
{
	return (
		m_Table != rOther.m_Table ||
		m_BucketIndex != rOther.m_BucketIndex ||
		m_ElementIndex != rOther.m_ElementIndex
		);
}

template<class Table, bool Const>
bool HashTableIterator<Table, Const>::operator<(const HashTableIterator& rOther) const
{
	ASSERT(m_Table == rOther.m_Table);
	return (
		m_Table == rOther.m_Table &&
		(m_BucketIndex < rOther.m_BucketIndex ||
			(m_BucketIndex == rOther.m_BucketIndex && m_ElementIndex < rOther.m_ElementIndex))
		);
}

template<class Table, bool Const>
bool HashTableIterator<Table, Const>::operator>(const HashTableIterator& rOther) const
{
	ASSERT(m_Table == rOther.m_Table);
	return (
		m_Table == rOther.m_Table &&
		(m_BucketIndex > rOther.m_BucketIndex ||
			(m_BucketIndex == rOther.m_BucketIndex && m_ElementIndex > rOther.m_ElementIndex))
		);
}

template<class Table, bool Const>
bool HashTableIterator<Table, Const>::operator<=(const HashTableIterator& rOther) const
{
	ASSERT(m_Table == rOther.m_Table);
	return (
		m_Table == rOther.m_Table &&
		(m_BucketIndex <= rOther.m_BucketIndex ||
			(m_BucketIndex == rOther.m_BucketIndex && m_ElementIndex <= rOther.m_ElementIndex))
		);
}

template<class Table, bool Const>
bool HashTableIterator<Table, Const>::operator>=(const HashTableIterator& rOther) const
{
	ASSERT(m_Table == rOther.m_Table);
	return (
		m_Table == rOther.m_Table &&
		(m_BucketIndex >= rOther.m_BucketIndex ||
			(m_BucketIndex == rOther.m_BucketIndex && m_ElementIndex >= rOther.m_ElementIndex))
		);
}


// HashTable functions
//------------------------------------------------------------------------------

// Constructors
//------------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::HashTable(
	SIZET bucketCount,
	const HashFunction& rHasher,
	const EqualKey& rKeyEquals,
	const ExtractKey& rExtractKey,
	const Allocator& rAllocator)
	: m_Size(0)
	, m_Hasher(rHasher)
	, m_KeyEquals(rKeyEquals)
	, m_ExtractKey(rExtractKey)
	, m_Allocator(rAllocator)
{
	m_Buckets.SetSize(bucketCount);
}

template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::HashTable(
	SIZET bucketCount,
	const HashFunction& rHasher,
	const EqualKey& rKeyEquals,
	const Allocator& rAllocator)
	: m_Size(0)
	, m_Hasher(rHasher)
	, m_KeyEquals(rKeyEquals)
	, m_Allocator(rAllocator)
{
	m_Buckets.SetSize(bucketCount);
}

// Copy constructors
//------------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::HashTable(const HashTable& rSource)
{
	CopyConstruct(rSource);
}

template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
template<class OtherAllocator>
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::HashTable(
	const HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, OtherAllocator>& rSource)
{
	CopyConstruct(rSource);
}

template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::HashTable(HashTable && rOther)	
	: m_Size(rOther.m_Size)
	, m_Buckets(std::move(rOther.m_Buckets))
	, m_Hasher(std::move(rOther.m_Hasher))
	, m_KeyEquals(std::move(rOther.m_KeyEquals))
	, m_ExtractKey(std::move(rOther.m_ExtractKey))
	, m_Allocator(std::move(rOther.m_Allocator))
{
	rOther.m_Size = 0;
}

// Destructor
//------------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::~HashTable()
{
}

// GetSize
//------------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
SIZET HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::GetSize() const
{
	return m_Size;
}

// IsEmpty
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
bool HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::IsEmpty() const
{
	return (m_Size == 0);
}

// Clear
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
void HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Clear()
{
	for (SIZET bucketIdx = 0; bucketIdx < m_Buckets.GetSize(); ++bucketIdx)
	{
		m_Buckets[bucketIdx].Clear();
	}
	m_Buckets.Clear();
	m_Size = 0;
}

// Trim
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
void HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Shrink()
{
	for (Array<Bucket>::Iter iter = m_Buckets.Begin(); iter != m_Buckets.End(); ++iter)
	{
		(*iter).Shrink();
	}
	m_Buckets.Shrink();
}

// Begin
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Iter 
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Begin()
{
	if (m_Size != 0)
	{
		for (SIZET bucketIdx = 0; bucketIdx < m_Buckets.GetSize(); ++bucketIdx)
		{
			if (!m_Buckets[bucketIdx].IsEmpty())
			{
				return Iter(this, bucketIdx, 0);
			}
		}
	}
	return End();
}

template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::ConstIter
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Begin() const
{
	if (m_Size != 0)
	{
		for (SIZET bucketIdx = 0; bucketIdx < m_Buckets.GetSize(); ++bucketIdx)
		{
			if (!m_Buckets[bucketIdx].IsEmpty())
			{
				return ConstIter(this, bucketIdx, 0);
			}
		}
	}
	return End();
}

// End
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Iter
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::End()
{
	return Iter(this, m_Buckets.GetSize(), 0);
}

template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::ConstIter
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::End() const
{
	return ConstIter(this, m_Buckets.GetSize(), 0);
}

// Last
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Iter
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Last()
{
	return --End();
}

template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::ConstIter
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Last() const
{
	return --End();
}


// Find
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Iter 
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Find(const Key& rKey)
{
	if (m_Size != 0)
	{
		SIZET bucketIndex = (m_Hasher(rKey) % m_Buckets.GetSize());
		Bucket& rEntries = m_Buckets[bucketIndex];
		SIZET entryCount = rEntries.GetSize();
		for (SIZET entryIndex = 0; entryIndex < entryCount; ++entryIndex)
		{
			if (m_KeyEquals(m_ExtractKey(rEntries[entryIndex]), rKey))
			{
				return Iter(this, bucketIndex, entryIndex);
			}
		}
	}

	return End();
}

template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::ConstIter
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Find(const Key& rKey) const
{
	if (m_Size != 0)
	{
		SIZET bucketIndex = (m_Hasher(rKey) % m_Buckets.GetSize());
		Bucket& rEntries = m_Buckets[bucketIndex];
		SIZET entryCount = rEntries.GetSize();
		for (SIZET entryIndex = 0; entryIndex < entryCount; ++entryIndex)
		{
			if (m_KeyEquals(m_ExtractKey(rEntries[entryIndex]), rKey))
			{
				return ConstIter(this, bucketIndex, entryIndex);
			}
		}
	}

	return End();
}


// Insert (unique)
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
Pair<typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Iter, bool>
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::InsertUnique(const Value& rValue)
{
	Pair<Iter, bool> result;
	result.Second() = InsertUnique(result.First(), rValue);

	return result;
}


template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
Pair<typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Iter, bool>
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::InsertUnique(Value&& rValue)
{
	Pair<Iter, bool> result;
	result.Second() = InsertUnique(result.First(), std::move(rValue));

	return result;
}


template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
bool HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::InsertUnique(
	Iter& rIterator,
	const Value& rValue)
{
	return _InsertUnique(rIterator, rValue);
}


template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
bool HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::InsertUnique(
	Iter& rIterator,
	Value&& rValue)
{
	return _InsertUnique(rIterator, std::move(rValue));
}

// Insert (equal)
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Iter
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::InsertEqual(const Value& rValue)
{
	ASSERT(m_Buckets);
	const Key& rKey = m_ExtractKey(rValue);
	SIZET bucketIndex = m_Hasher(rKey) % m_Buckets.GetSize();

	// Search for an existing entry.
	Bucket& rEntries = m_Buckets[bucketIndex];
	SIZET entryCount = rEntries.GetSize();
	for (SIZET entryIndex = 0; entryIndex < entryCount; ++entryIndex)
	{
		if (m_KeyEquals(m_ExtractKey(rEntries[entryIndex]), rKey))
		{
			rEntries.Insert(entryIndex + 1, rValue);
			++m_Size;
			return Iter(this, bucketIndex, entryIndex + 1);
		}
	}

	// Entry not found, so add it to the table.
	rEntries.Append(rValue);
	++m_Size;

	return Iter(this, bucketIndex, entryCount);
}

// FindOrInsert
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
Value & HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::FindOrInsert(
	const Value& rValue)
{
	Pair<Iter, bool> result;
	result = InsertUnique(rValue);
	return *result.First();
}

// Remove
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
SIZET HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Erase(const Key& rKey)
{
	SIZET bucketIndex = m_Hasher(rKey) % m_Buckets.GetSize();
	Bucket& rEntries = m_Buckets[bucketIndex];
	SIZET entryCount = rEntries.GetSize();

	SIZET erasedCount = 0;

	for (SIZET entryIndex = 0; entryIndex < entryCount; ++entryIndex)
	{
		if (m_KeyEquals(m_ExtractKey(rEntries[entryIndex]), rKey))
		{
			rEntries.EraseSwapIndex(entryIndex);
			++erasedCount;
			--entryCount;
		}
	}

	m_Size -= erasedCount;
	return erasedCount;
}

template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Iter 
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Erase(ConstIter& iterator)
{
	ASSERT(iterator.m_Table == this);
	ASSERT(iterator.m_ElementIndex < m_Buckets[iterator.m_BucketIndex].GetSize());
	m_Buckets[iterator.m_BucketIndex].EraseSwapIndex(iterator.m_ElementIndex);
	--m_Size;
	return iterator;
}

// Swap
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
void HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Swap(HashTable& rTable)
{
	std::swap(m_Buckets, rTable.m_Buckets);
	std::swap(m_Size, rTable.m_Size);
	std::swap(m_Hasher, rTable.m_Hasher);
	std::swap(m_KeyEquals, rTable.m_KeyEquals);
	std::swap(m_ExtractKey, rTable.m_ExtractKey);
	std::swap(m_Allocator, rTable.m_Allocator);
}

// Operators
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>&
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::operator=(const HashTable& rSource)
{
	if (this != &rSource)
	{
		CopyConstruct(rSource);
	}

	return *this;
}

template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
template<class OtherAllocator>
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>&
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::operator=(
	const HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, OtherAllocator>& rSource)
{
	if (this != &rSource)
	{
		CopyConstruct(rSource);
	}

	return *this;
}

template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>&
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::operator=(HashTable&& rOther)
{
	Swap(rOther);
	return *this;
}

template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
void HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::ReHash(SIZET size)
{
	Array<Bucket> newBucket(size);

	for (SIZET bucketIdx = 0; bucketIdx < m_Buckets.GetSize(); ++bucketIdx)
	{
		Bucket& bucket = m_Buckets[bucketIdx];
		for (Bucket::Iter iter = bucket.Begin(); iter != bucket.End(); ++iter)
		{
			const Key& rKey = m_ExtractKey(*iter);
			SIZET newBucketIndex = m_Hasher(rKey) % size;
			newBucket[newBucketIndex].EmplaceAppend(std::move(*iter));
		}
	}

	m_Buckets = std::move(newBucket);

	// TODO in-place rehash
// 	m_Buckets.SetSize(size);
// 	Array<SIZET> indexArr(size, 0);
// 	for (SIZET bucketIdx = 0; bucketIdx < m_Buckets.GetSize(); ++bucketIdx)
// 	{
// 		Bucket& bucket = m_Buckets[bucketIdx];
// 		for (SIZET idx = indexArr[bucketIdx]; idx < bucket.GetSize();)
// 		{
// 			const Key& rKey = m_ExtractKey(bucket[idx]);
// 			SIZET newBucketIndex = m_Hasher(rKey) % size;
// 			SIZET oldElementStart = indexArr[newBucketIndex];
// 
// 			Bucket& newBucket = m_Buckets[newBucketIndex];
// 			if (newBucketIndex == bucketIdx)
// 			{
// 				// rehash to same bucket
// 				ASSERT(oldElementStart <= idx);
// 				if (oldElementStart < idx)
// 				{
// 					std::swap(newBucket[oldElementStart], bucket[idx]);
// 				}
// 				++idx;
// 			}
// 			else
// 			{
// 				// rehash to other bucket
// 				if (newBucket.IsEmpty() || newBucket.GetSize() <= oldElementStart)
// 				{
// 					newBucket.EmplaceAppend(std::move(bucket[idx]));
// 					++idx;
// 				}
// 				else
// 				{
// 					std::swap(newBucket[oldElementStart], bucket[idx]);
// 				}
// 			}
// 			++indexArr[newBucketIndex];
// 		}
// 		bucket.SetSize(indexArr[bucketIdx]);
// 	}
}

// _InsertUnique
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
template<class ValueT>
bool HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::_InsertUnique(Iter& rIterator, ValueT&& rValue)
{
	const Key& rKey = m_ExtractKey(rValue);
	SIZET bucketSize = m_Buckets.GetSize();
	SIZET bucketIndex = m_Hasher(rKey) % bucketSize;

	if (m_Size >= bucketSize * DEFAULT_LOAD_FACTOR)
	{
		ReHash(bucketSize << 1);
	}

	// Search for an existing entry.
	Bucket& rEntries = m_Buckets[bucketIndex];
	SIZET entryCount = rEntries.GetSize();
	for (SIZET entryIndex = 0; entryIndex < entryCount; ++entryIndex)
	{
		if (m_KeyEquals(m_ExtractKey(rEntries[entryIndex]), rKey))
		{
			rIterator = Iter(this, bucketIndex, entryIndex);

			return false;
		}
	}

	// Entry not found, so add it to the table.
	rEntries.EmplaceAppend(std::forward<ValueT>(rValue));
	++m_Size;

	rIterator = Iter(this, bucketIndex, entryCount);

	return true;
}

// CopyConstruct
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
template<class OtherAllocator>
void HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::CopyConstruct(
	const HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, OtherAllocator>& rSource)
{
	m_Size = rSource.m_Size;
	m_Hasher = rSource.m_Hasher;
	m_KeyEquals = rSource.m_KeyEquals;
	m_ExtractKey = rSource.m_ExtractKey;
	m_Buckets = rSource.m_Buckets;
}

//-----------------------------------------------------------------------------