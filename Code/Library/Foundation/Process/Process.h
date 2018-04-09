// Process.h
//------------------------------------------------------------------------------
#pragma once
#ifndef CORE_PROCESS_H
#define CORE_PROCESS_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Container/SmartPtr.h"

// Process
//------------------------------------------------------------------------------
class Process
{
public:
	Process();
	~Process();

	bool Spawn(const char * executable,
		const char * args,
		const char * workingDir,
		const char * environment,
		bool shareHandles = false);
	bool IsRunning() const;	
	void Detach();
	int WaitForExit();

	// Read all data from the process until it exits
	// NOTE: Owner must free the returned memory!
	void ReadAllData(AutoPtr<char> & memOut, uint32 * memOutSize, AutoPtr<char> & errOut, uint32 * errOutSize);

#if defined(__WINDOWS__)
	// Read all available data
	// NOTE: Owner must free the returned memory!
	char * ReadStdOut(uint32 * bytesRead = nullptr);
	char * ReadStdErr(uint32 * bytesRead = nullptr);

	// read available data into a limited user buffer
	uint32 ReadStdOut(char * outputBuffer, uint32 outputBufferSize);
	uint32 ReadStdErr(char * outputBuffer, uint32 outputBufferSize);

	// Prevent handles being redirected
	inline void DisableHandleRedirection() { m_RedirectHandles = false; }
#endif

	static uint32 GetCurrentId();
private:
#if defined(__WINDOWS__)
	void   Read(void * handle, AutoPtr<char> & buffer, uint32 & sizeSoFar, uint32 & bufferSize);
	char * Read(void * handle, uint32 * bytesRead);
	uint32 Read(void * handle, char * outputBuffer, uint32 outputBufferSize);
#else
	void Read(int handle, AutoPtr<char> & buffer, uint32 & sizeSoFar, uint32 & bufferSize);
#endif

#if defined(__WINDOWS__)
	// This messyness is to avoid including windows.h in this file
	inline struct _PROCESS_INFORMATION & GetProcessInfo() const
	{
		return (_PROCESS_INFORMATION &)m_ProcessInfo;
	}
#endif

#if defined (__WIN64__)
	uint32 m_ProcessInfo[2 + 2 + 1 + 1]; // PROCESS_INFORMATION
#elif defined (__WIN32__)
	uint32 m_ProcessInfo[1 + 1 + 1 + 1]; // PROCESS_INFORMATION
#endif

	bool m_Started;
#if defined(__WINDOWS__)
	bool m_SharingHandles;
	bool m_RedirectHandles;
#endif

#if defined(__WINDOWS__)
	void * m_StdOutRead;	// HANDLE
	void * m_StdOutWrite;	// HANDLE
	void * m_StdErrRead;	// HANDLE
	void * m_StdErrWrite;	// HANDLE
#endif

#if defined(__LINUX__) || defined(__APPLE__)
	int m_ChildPID;
	mutable bool m_HasAlreadyWaitTerminated;
	mutable int m_ReturnStatus;
	int m_StdOutRead;
	int m_StdErrRead;
#endif
};

//------------------------------------------------------------------------------
#endif // CORE_PROCESS_H
