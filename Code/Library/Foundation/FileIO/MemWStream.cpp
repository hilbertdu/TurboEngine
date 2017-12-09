// MemoryStream.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/FileIO/MemWStream.h"
#include "Foundation/Memory/Mem.h"

// Defines
//------------------------------------------------------------------------------

// CONSTRUCTOR
//------------------------------------------------------------------------------
MemWStream::MemWStream()
	: m_Begin(nullptr)
	, m_End(nullptr)
	, m_MaxEnd(nullptr)
	, m_MinGrowth(4096)
{
}

// CONSTRUCTOR
//------------------------------------------------------------------------------
MemWStream::MemWStream(SIZET initialBufferSize, SIZET minGrowthFactor)
	: m_Begin((char *)ALLOC(initialBufferSize))
	, m_End(m_Begin)
	, m_MaxEnd(m_Begin + initialBufferSize)
	, m_MinGrowth(minGrowthFactor)
{
}

// DESTRUCTOR
//------------------------------------------------------------------------------
MemWStream::~MemWStream()
{
	FREE(m_Begin);
}

// Read
//------------------------------------------------------------------------------
uint64 MemWStream::ReadBuffer(void * buffer, uint64 bytesToRead) const
{
	(void)buffer; (void)bytesToRead;
	ASSERT(false); // Not implemented - implement if required
	return 0;
}

// Write
//------------------------------------------------------------------------------
uint64 MemWStream::WriteBuffer(const void * buffer, uint64 bytesToWrite)
{
	if ((m_End + bytesToWrite) > m_MaxEnd)
	{
		GrowToAccomodate(bytesToWrite);
	}

	MemCopy(m_End, buffer, (SIZET)bytesToWrite);
	m_End += bytesToWrite;

	return bytesToWrite;
}

// Flush
//------------------------------------------------------------------------------
void MemWStream::Flush()
{
	// nothing to do
}

// Tell
//------------------------------------------------------------------------------
uint64 MemWStream::Tell() const
{
	ASSERT(false); // Not implemented - implement if required
	return 0;
}

// Seek
//------------------------------------------------------------------------------
bool MemWStream::Seek(uint64 pos) const
{
	(void)pos;
	ASSERT(false); // Not implemented - implement if required
	return true;
}

// Grow
//------------------------------------------------------------------------------
void MemWStream::GrowToAccomodate(uint64 bytesToAccomodate)
{
	// grow by at least MinGrowth
	SIZET newCapacity = (m_MaxEnd - m_Begin) + Math::Max<SIZET>((SIZET)bytesToAccomodate, m_MinGrowth);
	char * oldBegin = m_Begin;
	char * oldEnd = m_End;
	m_Begin = (char *)ALLOC(newCapacity);
	m_End = m_Begin + (oldEnd - oldBegin);
	m_MaxEnd = m_Begin + newCapacity;
	MemCopy(m_Begin, oldBegin, oldEnd - oldBegin);
	FREE(oldBegin);
}

//------------------------------------------------------------------------------
