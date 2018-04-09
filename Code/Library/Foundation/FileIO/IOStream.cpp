// IOStream - interface for serialization
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/FileIO/IOStream.h"
#include "Foundation/Math/Conversion.h"
#include "Foundation/String/String.h"

// Read (AString)
//------------------------------------------------------------------------------
bool IOStream::Read(AString & string) const
{
	uint32 len;
	if (Read(len))
	{
		string.SetCapacity(len);
		return (Read(string.Get(), len) == len);
	}
	return false;
}

// Write (AString)
//------------------------------------------------------------------------------
bool IOStream::Write(const AString & string)
{
	uint32 len = string.GetLength();
	bool ok = Write(len);
	ok &= (Write(string.Get(), len) == len);
	return ok;
}

// AlignRead
//------------------------------------------------------------------------------
void IOStream::AlignRead(SIZET alignment)
{
	const uint64 tell = Tell();
	const uint64 toSkip = Math::RoundUp(tell, (uint64)alignment) - tell;
	for (uint64 i = 0; i < toSkip; ++i)
	{
		uint8 tmp;
		Read(tmp);
	}
	ASSERT((Tell() % alignment) == 0);
}

// AlignWrite
//------------------------------------------------------------------------------
void IOStream::AlignWrite(SIZET alignment)
{
	const uint64 tell = Tell();
	const uint64 toPad = Math::RoundUp(tell, (uint64)alignment) - tell;
	for (uint64 i = 0; i < toPad; ++i)
	{
		uint8 padChar(0);
		Write(padChar);
	}
	ASSERT((Tell() % alignment) == 0);
}

// ReadLine
//------------------------------------------------------------------------------
void IOStream::ReadLine(AString & string, char delim) const
{
	string.Clear();

	char c;
	while (Read(&c, 1) == 1 && c != delim)
	{
		string += c;
	}
}

//------------------------------------------------------------------------------
