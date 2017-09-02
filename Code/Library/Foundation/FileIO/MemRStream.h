// ConstMemoryStream - read only access to data wrapped in an IOStream interface
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_FILEIO_MEMRSTREAM_H
#define FOUNDATION_FILEIO_MEMRSTREAM_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/FileIO/IOStream.h"

// ConstMemoryStream
//------------------------------------------------------------------------------
class MemRStream : public IOStream
{
public:
	explicit MemRStream(const void * data, SIZET size);
	~MemRStream();

	// memory stream specific functions
	inline const void * GetData() const { return m_Buffer; }
	virtual const void*	GetCurData() const { return (char*)m_Buffer + m_CurrentPos; }
	inline SIZET		GetSize() const { return m_Size; }
	void				Replace(const void * data, SIZET size);

	// raw read/write functions
	virtual uint64 ReadBuffer(void * buffer, uint64 bytesToRead) const;
	virtual uint64 WriteBuffer(const void * buffer, uint64 bytesToWrite);
	virtual void Flush();

	// size/position
	virtual uint64 Tell() const;
	virtual bool Seek(uint64 pos) const;
	virtual uint64 GetFileSize() const;

private:
	const void * m_Buffer;
	SIZET m_Size;
	mutable SIZET m_CurrentPos;
};

//------------------------------------------------------------------------------
#endif // FOUNDATION_FILEIO_MEMRSTREAM_H
