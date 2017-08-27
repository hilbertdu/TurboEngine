// MemoryStream
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_FILEIO_MEMORYIOSTREAM_H
#define FOUNDATION_FILEIO_MEMORYIOSTREAM_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/FileIO/IOStream.h"

// MemoryStream
//------------------------------------------------------------------------------
class MemoryStream : public IOStream
{
public:
	explicit MemoryStream();
	explicit MemoryStream(SIZET initialBufferSize, SIZET minGrowth = 4096);
	~MemoryStream();

	// memory stream specific functions
	inline const void * GetData() const { return (void *)m_Begin; }
	inline SIZET		GetSize() const { return (m_End - m_Begin); }

	// raw read/write functions
	virtual uint64 ReadBuffer(void * buffer, uint64 bytesToRead) const;
	virtual uint64 WriteBuffer(const void * buffer, uint64 bytesToWrite);
	virtual void Flush();

	// size/position
	virtual uint64 Tell() const;
	virtual bool Seek(uint64 pos) const;

private:
	NO_INLINE void GrowToAccomodate(uint64 bytesToAccomodate);

	char *			m_Begin;
	char *			m_End;
	char *			m_MaxEnd;
	SIZET			m_MinGrowth;
};

//------------------------------------------------------------------------------
#endif // FOUNDATION_FILEIO_MEMORYIOSTREAM_H
