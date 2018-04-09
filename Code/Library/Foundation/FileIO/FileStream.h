// FileStream
//------------------------------------------------------------------------------
#pragma once
#ifndef CORE_FILEIO_FILESTREAM_H
#define CORE_FILEIO_FILESTREAM_H

//------------------------------------------------------------------------------
#include "IOStream.h"
#include <WinSock2.h>
#include <Windows.h>

// FileStream
//------------------------------------------------------------------------------
class FileStream : public IOStream
{
public:
	explicit FileStream();
	~FileStream();

	enum FileMode
	{
		READ_ONLY = 0x1,
		WRITE_ONLY = 0x2,
		TEMP = 0x4,
		DELETE_ON_CLOSE = 0x8,
		SHARE_READ = 0x10,
		SHARE_WRITE = 0x20,
		SHARE_DELETE = 0x40,
		NO_RETRY_ON_SHARING_VIOLATION = 0x80,
	};

	bool Open(const char * fileName, uint32 mode = FileStream::READ_ONLY);
	void Close();

	bool IsOpen() const;

	// raw read/write functions
	virtual uint64	ReadBuffer(void * buffer, uint64 bytesToRead) const;
	virtual uint64	WriteBuffer(const void * buffer, uint64 bytesToWrite);
	virtual void	Flush();

	void ReadTotalUTF16(WCHAR * buffer);
	void WriteUTF16(const WCHAR * buffer, uint64 size);
	void WriteUTF16Head();

	// size/position
	virtual uint64		Tell() const;
	virtual bool		Seek(uint64 pos) const;
	virtual uint64		GetFileSize() const;
	virtual SIZET		GetSize() const { return (SIZET)GetFileSize(); }
	virtual const void* GetCurData() const { return nullptr; }

	// file specific
	bool SetLastWriteTime(uint64 lastWriteTime);

	// Access to handle
	inline void * GetHandle() const { return m_Handle; }	

private:
	void * m_Handle;
};

//------------------------------------------------------------------------------
#endif // CORE_FILEIO_FILESTREAM_H
