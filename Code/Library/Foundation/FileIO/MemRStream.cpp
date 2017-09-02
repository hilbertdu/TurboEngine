// MemRStream.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/FileIO/MemRStream.h"
#include <memory.h> // for memcpy

// Defines
//------------------------------------------------------------------------------

// CONSTRUCTOR
//------------------------------------------------------------------------------
MemRStream::MemRStream(const void * data, SIZET size)
	: m_Buffer(data)
	, m_Size(size)
	, m_CurrentPos(0)
{
}

// DESTRUCTOR
//------------------------------------------------------------------------------
MemRStream::~MemRStream()
{
}

// Replace
//------------------------------------------------------------------------------
void MemRStream::Replace(const void * data, SIZET size)
{
	m_Buffer = data;
	m_Size = size;
	m_CurrentPos = 0;
}

// Read
//------------------------------------------------------------------------------
uint64 MemRStream::ReadBuffer(void * buffer, uint64 bytesToRead) const
{
	SIZET availBytes = (m_Size - m_CurrentPos);
	SIZET actualBytesToRead = Math::Min(availBytes, (SIZET)bytesToRead);
	if (buffer)
	{
		memcpy(buffer, (void *)((SIZET)m_Buffer + m_CurrentPos), actualBytesToRead);
	}
	m_CurrentPos += actualBytesToRead;
	return actualBytesToRead;
}

// Write
//------------------------------------------------------------------------------
uint64 MemRStream::WriteBuffer(const void *, uint64)
{
	ASSERT(false); // not permitted
	return 0;
}

// Flush
//------------------------------------------------------------------------------
void MemRStream::Flush()
{
	ASSERT(false); // not permitted
}

// Tell
//------------------------------------------------------------------------------
uint64 MemRStream::Tell() const
{
	return m_CurrentPos;
}

// Seek
//------------------------------------------------------------------------------
bool MemRStream::Seek(uint64 pos) const
{
	if ((SIZET)pos > m_Size)
	{
		return false;
	}
	m_CurrentPos = (SIZET)pos;
	return true;
}

// GetFileSize
//------------------------------------------------------------------------------
uint64 MemRStream::GetFileSize() const
{
	return m_Size;
}

//------------------------------------------------------------------------------
