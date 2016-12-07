// HashTable.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_CONTAINER_PAIR_H
#define FOUNDATION_CONTAINER_PAIR_H

// Include
//------------------------------------------------------------------------------


// Pair
//------------------------------------------------------------------------------
template<typename T1, typename T2>
class Pair
{
public:
	typedef T1 FirstType;
	typedef T2 SecondType;

	explicit Pair() = default;
	Pair(const T1& rFirst, const T2& rSecond);	// not explicit to support initializer list ?
	Pair(const Pair<T1, T2>& rPair) = default;
	Pair& operator=(const Pair<T1, T2>& rOther) = default;
	bool  operator==(const Pair& rOther) { return m_First == rOther.m_First && m_Second == rOther.m_Second; };
	bool  operator!=(const Pair& rOther) { return m_First != rOther.m_First || m_Second != rOther.m_Second; };

	inline T1& First() { return m_First; };
	inline T2& Second() { return m_Second; };

	inline const T1& First() const { return m_First; };
	inline const T2& Second() const { return m_Second; };

protected:
	T1 m_First;
	T2 m_Second;
};


// KeyValuePair
//------------------------------------------------------------------------------
template<typename T1, typename T2>
class KeyValuePair : public Pair<T1, T2>
{
public:
	explicit KeyValuePair() = default;
	KeyValuePair(const T1& rFirst, const T2& rSecond) : Pair(rFirst, rSecond) {};
	KeyValuePair(const Pair<T1, T2>& rPair) : Pair(rPair) {};

	inline const T1& First() { return m_First; };
	inline const T1& First() const { return m_First; };
};


// SelectFirst
//------------------------------------------------------------------------------
template<typename PairType>
class SelectFirst
{
public:
	typename PairType::FirstType& operator()(PairType& rPair) const;
	const typename PairType::FirstType& operator()(const PairType& rPair) const;
};


// SelectKey
//------------------------------------------------------------------------------
template<typename PairType>
class SelectKey
{
public:
	const typename PairType::FirstType& operator()(const PairType& rPair) const;
};


// Pair class member
//------------------------------------------------------------------------------
template<typename T1, typename T2>
Pair<T1, T2>::Pair(const T1& rFirst, const T2& rSecond)
	: m_First(rFirst)
	, m_Second(rSecond)
{
}


// SelectFirst class member
//------------------------------------------------------------------------------
template<typename PairType>
typename PairType::FirstType& SelectFirst<PairType>::operator()(PairType& rPair) const
{
	return rPair.First();
}

template<typename PairType>
const typename PairType::FirstType& SelectFirst<PairType>::operator()(const PairType& rPair) const
{
	return rPair.First();
}

// SelectKey class member
//------------------------------------------------------------------------------
template<typename PairType>
const typename PairType::FirstType& SelectKey<PairType>::operator()(const PairType& rPair) const
{
	return rPair.First();
}


#endif // FOUNDATION_CONTAINER_PAIR_H
