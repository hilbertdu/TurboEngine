// StrStream.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/FileIO/StrStream.h"

// Defines
//------------------------------------------------------------------------------

// CONSTRUCTOR
//------------------------------------------------------------------------------
StrStream::StrStream(SIZET minGrowth)
	: m_Buffer(nullptr)
	, m_RPos(0)
	, m_WPos(0)
	, m_Capacity(0)
	, m_MinGrowth(minGrowth)
{
}

StrStream::StrStream(const char * data)
	: m_RPos(0)
{
	m_Capacity = StringHelper::StrLen(data) + 1;
	m_Buffer = (char*)ALLOC(m_Capacity);
	StringHelper::Copy(data, m_Buffer, m_Capacity - 1);	
	m_RPos = m_Capacity - 1;
}

// DESTRUCTOR
//------------------------------------------------------------------------------
StrStream::~StrStream()
{
	TDELETE_SAFE(m_Buffer);
}

// Read
//------------------------------------------------------------------------------
uint64 StrStream::ReadBuffer(void * buffer, uint64 bytesToRead) const
{
	SIZET availBytes = (m_WPos - m_RPos);
	SIZET actualBytesToRead = Math::Min(availBytes, (SIZET)bytesToRead);
	if (buffer)
	{
		MemCopy(buffer, (void *)((SIZET)m_Buffer + m_RPos), actualBytesToRead);
	}
	m_RPos += actualBytesToRead;
	return actualBytesToRead;
}

// Write
//------------------------------------------------------------------------------
uint64 StrStream::WriteBuffer(const void * buffer, uint64 bytesToWrite)
{
	if ((m_WPos + bytesToWrite) > m_Capacity)
	{
		GrowToAccomodate(bytesToWrite);
	}

	MemCopy((void *)((SIZET)m_Buffer + m_WPos), buffer, (SIZET)bytesToWrite);
	m_WPos += bytesToWrite;

	return bytesToWrite;
}

// Flush
//------------------------------------------------------------------------------
void StrStream::Flush()
{
	ASSERT(false);
}

// Tell
//------------------------------------------------------------------------------
uint64 StrStream::Tell() const
{
	return m_WPos;
}

// Seek
//------------------------------------------------------------------------------
bool StrStream::Seek(uint64 pos) const
{
	if ((SIZET)pos > m_WPos)
	{
		return false;
	}
	m_RPos = (SIZET)pos;
	return true;
}

// ReadStr
//------------------------------------------------------------------------------
/*virtual*/ bool StrStream::Read(int64 & i) const
{
	static constexpr const char * s_Matches = "0123456789";
	AStackString<> ustr;
	while (m_RPos < m_WPos)
	{
		if (StringHelper::Find(s_Matches, 10, m_Buffer[m_RPos]))
		{
			ustr += m_Buffer[m_RPos];
			m_RPos++;
		}
		else
		{
			break;
		}		
	}
	if (!ustr.IsEmpty())
	{
		i = _atoi64(ustr.Get());
		return true;
	}
	return false;
}

// ReadStr
//------------------------------------------------------------------------------
/*virtual*/ bool StrStream::Read(double & d) const
{
	static constexpr const char * s_Matches = ".0123456789";
	AStackString<> dstr;
	while (m_RPos < m_WPos)
	{
		if (StringHelper::Find(s_Matches, 11, m_Buffer[m_RPos]))
		{
			dstr += m_Buffer[m_RPos];
			m_RPos++;
		}
		else
		{
			break;
		}
	}
	if (!dstr.IsEmpty())
	{
		d = atof(dstr.Get());
		return true;
	}
	return false;
}

/*virtual*/ bool StrStream::Write(const int64 & i)
{
	AStackString<> ustr;
	ustr.Format("%lld", i);
	return WriteBuffer(ustr.Get(), ustr.GetLength()) != 0;
}

/*virtual*/ bool StrStream::Write(const double & d)
{
	AStackString<> dstr;
	dstr.Format("%.2lf", d);
	return WriteBuffer(dstr.Get(), dstr.GetLength()) != 0;
}

// ReadStr
//------------------------------------------------------------------------------
/*virtual*/ bool StrStream::Read(AString & s, char delim) const
{
	s.Clear();
	while (m_RPos < m_WPos)
	{
		if (m_Buffer[m_RPos] != delim)
		{
			s += m_Buffer[m_RPos];
			m_RPos++;
		}
		else
		{
			break;
		}
	}
	return !s.IsEmpty();
}

// WriteStr
//------------------------------------------------------------------------------
/*virtual*/ bool StrStream::Write(const AString & s, char delim)
{
	uint64 bytes = WriteBuffer(s.Get(), s.GetLength());
	WriteBuffer(&delim, 1);
	return true;
}

// GrowToAccomodate
//------------------------------------------------------------------------------
void StrStream::GrowToAccomodate(uint64 bytesToAccomodate)
{
	// grow by at least MinGrowth
	m_Capacity += Math::Max<SIZET>((SIZET)bytesToAccomodate, m_MinGrowth);
	char * oldBuffer = m_Buffer;
	m_Buffer = (char *)ALLOC(m_Capacity);
	MemCopy(m_Buffer, oldBuffer, m_WPos);
	FREE(oldBuffer);
}

//------------------------------------------------------------------------------
