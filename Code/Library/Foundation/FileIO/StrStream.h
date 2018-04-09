// StrStream
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_FILEIO_STRSTREAM_H
#define FOUNDATION_FILEIO_STRSTREAM_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/FileIO/IOStream.h"
#include "Foundation/Math/Math.h"

// StrStream
//------------------------------------------------------------------------------
class StrStream : public IOStream
{
public:
	explicit StrStream(SIZET minGrowth = 4096);
	explicit StrStream(const char * data);	
	~StrStream();

	// memory stream specific functions
	inline const void * GetData() const { return m_Buffer; }
	virtual const void*	GetCurData() const { return (char*)m_Buffer + Math::Min(m_RPos, m_WPos); }
	inline SIZET		GetSize() const { return m_WPos; }

	// raw read/write functions
	virtual uint64	ReadBuffer(void * buffer, uint64 bytesToRead) const;
	virtual uint64	WriteBuffer(const void * buffer, uint64 bytesToWrite);
	virtual void	Flush();

	// size/position
	virtual bool	Seek(uint64 pos) const;
	virtual uint64	Tell() const;

	bool Read(int64 & i) const;
	bool Read(double & d) const;
	bool Read(AString & s, char delim = '\0') const;

	bool Write(const int64 & i);
	bool Write(const double & d);	
	bool Write(const AString & s, char delim = '\0');

private:
	NO_INLINE void GrowToAccomodate(uint64 bytesToAccomodate);

	char *			m_Buffer;	
	mutable SIZET	m_RPos;
	mutable SIZET	m_WPos;
	SIZET			m_Capacity;
	SIZET			m_MinGrowth;
};

//------------------------------------------------------------------------------
#endif // FOUNDATION_FILEIO_STRSTREAM_H
