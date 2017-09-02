// IOStream - interface for serialization
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_FILEIO_IOSTREAM_H
#define FOUNDATION_FILEIO_IOSTREAM_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Types.h"
#include "Foundation/Container/Array.h"
#include "Foundation/String/String.h"

// IStream
//------------------------------------------------------------------------------
class IOStream
{
public:
	explicit inline IOStream() {}
	inline virtual ~IOStream() {}

	// interface that must be implemented
	virtual uint64 ReadBuffer(void * buffer, uint64 bytesToRead) const = 0;
	virtual uint64 WriteBuffer(const void * buffer, uint64 bytesToWrite) = 0;
	virtual void Flush() = 0;

	// size/position
	virtual bool Seek(uint64 pos) const = 0;
	virtual uint64 Tell() const = 0;
	virtual SIZET GetSize() const = 0;
	virtual const void* GetCurData() const = 0;

	// helper read wrappers
	inline uint64 Read(void * b, SIZET s) const { return ReadBuffer(b, s); }
	inline bool Read(bool & b) const { return (ReadBuffer(&b, sizeof(b)) == sizeof(b)); }
	inline bool Read(int8 & i) const { return (ReadBuffer(&i, sizeof(i)) == sizeof(i)); }
	inline bool Read(int16 & i) const { return (ReadBuffer(&i, sizeof(i)) == sizeof(i)); }
	inline bool Read(int32 & i) const { return (ReadBuffer(&i, sizeof(i)) == sizeof(i)); }
	inline bool Read(int64 & i) const { return (ReadBuffer(&i, sizeof(i)) == sizeof(i)); }
	inline bool Read(uint8 & u) const { return (ReadBuffer(&u, sizeof(u)) == sizeof(u)); }
	inline bool Read(uint16 & u) const { return (ReadBuffer(&u, sizeof(u)) == sizeof(u)); }
	inline bool Read(uint32 & u) const { return (ReadBuffer(&u, sizeof(u)) == sizeof(u)); }
	inline bool Read(uint64 & u) const { return (ReadBuffer(&u, sizeof(u)) == sizeof(u)); }
	bool Read(AString & string) const;
	template< class T > inline bool Read(Array< T > & a) const;

	// helper write wrappers
	inline uint64 Write(const void * b, SIZET s) { return WriteBuffer(b, s); }
	inline bool Write(const bool & b) { return (WriteBuffer(&b, sizeof(b)) == sizeof(b)); }
	inline bool Write(const int8 & i) { return (WriteBuffer(&i, sizeof(i)) == sizeof(i)); }
	inline bool Write(const int16 & i) { return (WriteBuffer(&i, sizeof(i)) == sizeof(i)); }
	inline bool Write(const int32 & i) { return (WriteBuffer(&i, sizeof(i)) == sizeof(i)); }
	inline bool Write(const int64 & i) { return (WriteBuffer(&i, sizeof(i)) == sizeof(i)); }
	inline bool Write(const uint8 & u) { return (WriteBuffer(&u, sizeof(u)) == sizeof(u)); }
	inline bool Write(const uint16 & u) { return (WriteBuffer(&u, sizeof(u)) == sizeof(u)); }
	inline bool Write(const uint32 & u) { return (WriteBuffer(&u, sizeof(u)) == sizeof(u)); }
	inline bool Write(const uint64 & u) { return (WriteBuffer(&u, sizeof(u)) == sizeof(u)); }
	bool Write(const AString & string);
	template< class T > inline bool Write(const Array< T > & a);

	// manage memory-mapped aligned data
	void AlignRead(SIZET alignment);
	void AlignWrite(SIZET alignment);
};

// Read
//------------------------------------------------------------------------------
template<class T>
bool IOStream::Read(Array<T> & a) const
{
	uint32 num = 0;
	if (Read(num) == false) { return false; }
	a.SetSize(num);
	for (uint32 i = 0; i < num; ++i)
	{
		if (Read(a[i]) == false) { return false; }
	}
	return true;
}

// Write
//------------------------------------------------------------------------------
template< class T >
bool IOStream::Write(const Array<T> & a)
{
	uint32 num = (uint32)a.GetSize();
	if (Write(num) == false) { return false; }
	for (uint32 i = 0; i < num; ++i)
	{
		if (Write(a[i]) == false) { return false; }
	}
	return true;
}

//------------------------------------------------------------------------------
#endif // FOUNDATION_FILEIO_IOSTREAM_H
