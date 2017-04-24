// HashTableIterator functions
//------------------------------------------------------------------------------

// Constructors
//------------------------------------------------------------------------------
template<class Table, bool Const>
HashTableIterator<Table, Const>::HashTableIterator(Table* pTable, SIZET bucketIndex, SIZET elementIndex)
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
}

// Operators
//------------------------------------------------------------------------------
template<class Table, bool Const>
typename const HashTableIterator<Table, Const>::ValueType& HashTableIterator<Table, Const>::operator*() const
{
	ASSERT(m_Table);
	ASSERT(m_Table->m_Buckets);

	return m_Table->m_Buckets[m_BucketIndex][m_ElementIndex];
}

template<class Table, bool Const>
typename const HashTableIterator<Table, Const>::ValueType* HashTableIterator<Table, Const>::operator->() const
{
	ASSERT(m_Table);
	ASSERT(m_Table->m_Buckets);

	return &m_Table->m_Buckets[m_BucketIndex][m_ElementIndex];
}

template<class Table, bool Const>
template<class>
typename HashTableIterator<Table, Const>::ValueType& HashTableIterator<Table, Const>::operator*()
{
	ASSERT(m_Table);
	ASSERT(m_Table->m_Buckets);

	return m_Table->m_Buckets[m_BucketIndex][m_ElementIndex];
}

template<class Table, bool Const>
template<class>
typename HashTableIterator<Table, Const>::ValueType* HashTableIterator<Table, Const>::operator->()
{
	ASSERT(m_Table);
	ASSERT(m_Table->m_Buckets);

	return m_Table->m_Buckets[m_BucketIndex][m_ElementIndex];
}

template<class Table, bool Const>
HashTableIterator<Table, Const>& HashTableIterator<Table, Const>::operator++()
{
	ASSERT(m_Table);
	ASSERT(m_BucketIndex < m_Table->m_BucketCount);

	++m_ElementIndex;

	typename Table::Bucket* pBuckets = m_Table->m_Buckets;

	if (m_ElementIndex >= pBuckets[m_BucketIndex].GetSize())
	{
		m_ElementIndex = 0;

		SIZET bucketCount = m_Table->m_BucketCount;
		for (SIZET bucketIdx = m_BucketIndex + 1; bucketIdx < bucketCount; ++bucketIdx)
		{
			if (!pBuckets[bucketIdx].IsEmpty())
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
	ASSERT(m_BucketIndex <= m_Table->m_BucketCount);

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
	: m_Buckets(nullptr)
	, m_BucketCount(bucketCount)
	, m_Size(0)
	, m_Hasher(rHasher)
	, m_KeyEquals(rKeyEquals)
	, m_ExtractKey(rExtractKey)
	, m_Allocator(rAllocator)
{
	AllocateBuckets();
}

template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::HashTable(
	SIZET bucketCount,
	const HashFunction& rHasher,
	const EqualKey& rKeyEquals,
	const Allocator& rAllocator)
	: m_Buckets(nullptr)
	, m_BucketCount(bucketCount)
	, m_Size(0)
	, m_Hasher(rHasher)
	, m_KeyEquals(rKeyEquals)
	, m_Allocator(rAllocator)
{
	AllocateBuckets();
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
	: m_Buckets(rOther.m_Buckets)
	, m_BucketCount(rOther.m_BucketCount)
	, m_Size(rOther.m_Size)
	, m_Hasher(std::move(rOther.m_Hasher))
	, m_KeyEquals(std::move(rOther.m_KeyEquals))
	, m_ExtractKey(std::move(rOther.m_ExtractKey))
	, m_Allocator(std::move(rOther.m_Allocator))
{
	rOther.m_Buckets = nullptr;
	rOther.m_BucketCount = 0;
	rOther.m_Size = 0;
}

// Destructor
//------------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::~HashTable()
{
	Finalize();
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
	for (SIZET bucketIdx = 0; bucketIdx < m_BucketCount; ++bucketIdx)
	{
		m_Buckets[bucketIdx].Clear();
	}
	m_Size = 0;
}

// Trim
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
void HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Shrink()
{
	for (SIZET bucketIndex = 0; bucketIndex < m_BucketCount; ++bucketIndex)
	{
		m_Buckets[bucketIndex].Shrink();
	}
}

// Begin
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Iterator 
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Begin()
{
	if (m_Size != 0)
	{
		for (SIZET bucketIdx = 0; bucketIdx < m_BucketCount; ++bucketIdx)
		{
			if (!m_Buckets[bucketIdx].IsEmpty())
			{
				return Iterator(this, bucketIdx, 0);
			}
		}
	}
	return End();
}

template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::ConstIterator
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Begin() const
{
	if (m_Size != 0)
	{
		for (SIZET bucketIdx = 0; bucketIdx < m_BucketCount; ++bucketIdx)
		{
			if (!m_Buckets[bucketIdx].IsEmpty())
			{
				return ConstIterator(this, bucketIdx, 0);
			}
		}
	}
	return End();
}

// End
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Iterator
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::End()
{
	return Iterator(this, m_BucketCount, 0);
}

template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::ConstIterator
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::End() const
{
	return ConstIterator(this, m_BucketCount, 0);
}

// Find
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Iterator 
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Find(const Key& rKey)
{
	if (m_Size != 0)
	{
		SIZET bucketIndex = (m_Hasher(rKey) % m_BucketCount);

		ASSERT(m_Buckets);
		Bucket& rEntries = m_Buckets[bucketIndex];
		SIZET entryCount = rEntries.GetSize();
		for (SIZET entryIndex = 0; entryIndex < entryCount; ++entryIndex)
		{
			if (m_KeyEquals(m_ExtractKey(rEntries[entryIndex]), rKey))
			{
				return Iterator(this, bucketIndex, entryIndex);
			}
		}
	}

	return End();
}

template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::ConstIterator
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Find(const Key& rKey) const
{
	if (m_Size != 0)
	{
		SIZET bucketIndex = (m_Hasher(rKey) % m_BucketCount);

		ASSERT(m_Buckets);
		Bucket& rEntries = m_Buckets[bucketIndex];
		SIZET entryCount = rEntries.GetSize();
		for (SIZET entryIndex = 0; entryIndex < entryCount; ++entryIndex)
		{
			if (m_KeyEquals(m_ExtractKey(rEntries[entryIndex]), rKey))
			{
				return ConstIterator(this, bucketIndex, entryIndex);
			}
		}
	}

	return End();
}


// Insert (unique)
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
Pair<typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Iterator, bool>
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::InsertUnique(const Value& rValue)
{
	Pair<Iterator, bool> result;
	result.Second() = InsertUnique(result.First(), rValue);

	return result;
}


template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
bool HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::InsertUnique(
	Iterator& rIterator,
	const Value& rValue)
{
	const Key& rKey = m_ExtractKey(rValue);
	SIZET bucketIndex = m_Hasher(rKey) % m_BucketCount;

	// Search for an existing entry.
	ASSERT(m_Buckets);
	Bucket& rEntries = m_Buckets[bucketIndex];
	SIZET entryCount = rEntries.GetSize();
	for (SIZET entryIndex = 0; entryIndex < entryCount; ++entryIndex)
	{
		if (m_KeyEquals(m_ExtractKey(rEntries[entryIndex]), rKey))
		{
			rIterator = Iterator(this, bucketIndex, entryIndex);

			return false;
		}
	}

	// Entry not found, so add it to the table.
	rEntries.Append(rValue);
	++m_Size;

	rIterator = Iterator(this, bucketIndex, entryCount);

	return true;
}

// Insert (equal)
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Iterator
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::InsertEqual(const Value& rValue)
{
	ASSERT(m_Buckets);
	const Key& rKey = m_ExtractKey(rValue);
	SIZET bucketIndex = m_Hasher(rKey) % m_BucketCount;

	// Search for an existing entry.
	Bucket& rEntries = m_Buckets[bucketIndex];
	SIZET entryCount = rEntries.GetSize();
	for (SIZET entryIndex = 0; entryIndex < entryCount; ++entryIndex)
	{
		if (m_KeyEquals(m_ExtractKey(rEntries[entryIndex]), rKey))
		{
			rEntries.Insert(entryIndex + 1, rValue);
			++m_Size;
			return Iterator(this, bucketIndex, entryIndex + 1);
		}
	}

	// Entry not found, so add it to the table.
	rEntries.Append(rValue);
	++m_Size;

	return Iterator(this, bucketIndex, entryCount);
}

// FindOrInsert
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
Value & HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::FindOrInsert(
	const Value& rValue)
{
	Pair<Iterator, bool> result;
	result = InsertUnique(rValue);
	return *result.First();
}

// Remove
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
SIZET HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Erase(const Key& rKey)
{
	ASSERT(m_Buckets);
	SIZET bucketIndex = m_Hasher(rKey) % m_BucketCount;
	Bucket& rEntries = m_Buckets[bucketIndex];
	SIZET entryCount = rEntries.GetSize();

	SIZET erasedCount = 0;

	for (SIZET entryIndex = 0; entryIndex < entryCount; ++entryIndex)
	{
		if (m_KeyEquals(m_ExtractKey(rEntries[entryIndex]), rKey))
		{
			rEntries.EraseSwapIndex(entryIndex);
			++erasedCount;
			--entryIndex;
		}
	}

	m_Size -= erasedCount;
	return erasedCount;
}

template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
typename HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Iterator 
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Erase(ConstIterator& iterator)
{
	ASSERT(iterator.m_Table == this);
	ASSERT(iterator.m_BucketIndex < m_BucketCount);
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
	Bucket* pBuckets = m_Buckets;
	SIZET bucketCount = m_BucketCount;
	SIZET size = m_Size;
	HashFunction hasher = m_Hasher;
	EqualKey keyEquals = m_KeyEquals;
	ExtractKey extractKey = m_ExtractKey;
	Allocator allocator = m_Allocator;

	m_Buckets = rTable.m_Buckets;
	m_BucketCount = rTable.m_BucketCount;
	m_Size = rTable.m_Size;
	m_Hasher = rTable.m_Hasher;
	m_KeyEquals = rTable.m_KeyEquals;
	m_ExtractKey = rTable.m_ExtractKey;
	m_Allocator = rTable.m_Allocator;

	rTable.m_Buckets = pBuckets;
	rTable.m_BucketCount = bucketCount;
	rTable.m_Size = size;
	rTable.m_Hasher = hasher;
	rTable.m_KeyEquals = keyEquals;
	rTable.m_ExtractKey = extractKey;
	rTable.m_Allocator = allocator;
}

// Operators
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>&
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::operator=(const HashTable& rSource)
{
	if (this != &rSource)
	{
		Finalize();
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
		Finalize();
		CopyConstruct(rSource);
	}

	return *this;
}

template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>&
HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::operator=(HashTable&& rOther)
{
	std::swap(m_Buckets, rOther.m_Buckets)
	std::swap(m_BucketCount, rOther.m_BucketCount)
	std::swap(m_Size, rOther.m_Size)
	std::swap(m_Hasher, rOther.m_Hasher)
	std::swap(m_KeyEquals, rOther.m_KeyEquals)
	std::swap(m_ExtractKey, rOther.m_ExtractKey)
	std::swap(m_Allocator, rOther.m_Allocator)
}

template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
void HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::AllocateBuckets()
{
	m_Buckets = (Bucket *)m_Allocator.Allocate(sizeof(Bucket) * m_BucketCount);
	ASSERT(m_Buckets);

	Array<Bucket>::InPlaceConstruct(m_Buckets, m_BucketCount);
}

// CopyConstruct
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
template<class OtherAllocator>
void HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::CopyConstruct(
	const HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, OtherAllocator>& rSource)
{
	SIZET bucketCount = rSource.m_BucketCount;
	m_BucketCount = bucketCount;

	m_Size = rSource.m_Size;
	m_Hasher = rSource.m_Hasher;
	m_KeyEquals = rSource.m_KeyEquals;
	m_ExtractKey = rSource.m_ExtractKey;

	AllocateBuckets();

	for (SIZET bucketIndex = 0; bucketIndex < bucketCount; ++bucketIndex)
	{
		m_Buckets[bucketIndex] = rSource.m_Buckets[bucketIndex];
	}
}

// Finalize
//-----------------------------------------------------------------------------
template<class Value, class Key, class HashFunction, class ExtractKey, class EqualKey, class Allocator>
void HashTable<Value, Key, HashFunction, ExtractKey, EqualKey, Allocator>::Finalize()
{
	Array<Bucket>::InPlaceDestruct(m_Buckets, m_BucketCount);
	m_Allocator.Free(m_Buckets);
}
