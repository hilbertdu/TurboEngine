// FileIO.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_FILEIO_FILEIO_H
#define FOUNDATION_FILEIO_FILEIO_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Container/Array.h"
#include "Foundation/String/String.h"
#include "Foundation/String/StringView.h"

// Defines
//------------------------------------------------------------------------------
#define MAX_PATH_LEN 260

// FileIO
//------------------------------------------------------------------------------
class FileIO
{
public:
	static bool FileExists(const char * fileName);
	static bool FileDelete(const char * fileName);
	static bool FileCopy(const char * srcFileName, const char * dstFileName, bool allowOverwrite = true);
	static bool FileMove(const AString & srcFileName, const AString & dstFileName);

	// directory delete
	static bool DirectoryDelete(const AString & dirName, bool deleteSubdirs = true);

	// directory listing
	static bool GetFiles(const AStringView & path, const AStringView & wildCard, bool recurse, Array<AString> * results, bool includeDir = false);

	struct FileInfo
	{
		AString	m_Name;
		uint32	m_Attributes;
		uint64	m_LastWriteTime;
		uint64	m_Size;

		bool	IsReadOnly() const;
		bool	IsDirectory() const;
	};

	static bool GetFilesEx(const AStringView & path, const AStringView & wildCard, bool recurse, Array<FileInfo> * results, bool includeDir = false);
	static bool GetFileInfo(const AStringView & fileName, FileInfo & info);
	static void GetFileName(const AStringView & filePath, AString & fileName);

	static bool GetCurrentDir(AStringRef & output);
	static bool SetCurrentDir(const AString & dir);
	static bool GetTempDir(AString & output);
	static bool DirectoryCreate(const AString & path);
	static bool DirectoryExists(const AString & path);
	static bool EnsurePathExists(const AString & path);

	static bool CreateTempPath(const char * tempPrefix, AString & path);

	static uint64_t GetFileLastWriteTime(const AString & fileName);
	static bool		SetFileLastWriteTime(const AString & fileName, uint64_t fileTime);

	static bool		SetReadOnly(const char * fileName, bool readOnly);
    static bool     GetReadOnly(const char * fileName);
	static inline bool GetReadOnly(const AString & fileName) { return GetReadOnly(fileName.Get()); }

    #if defined(__WINDOWS__)
        static void	WorkAroundForWindowsFilePermissionProblem(const AString & fileName);
    #else
        FORCE_INLINE static void WorkAroundForWindowsFilePermissionProblem(const AString &) {}
    #endif

private:
	static void GetFilesRecurse(AStringRef & path, const AStringView & wildCard, Array<AString> * results, bool includeDir = false);
	static void GetFilesNoRecurse(const char * path, const char * wildCard, Array<AString> * results, bool includeDir = false);
	static void GetFilesRecurseEx(AStringRef & path, const AStringView & wildCard, Array<FileInfo> * results, bool includeDir = false);
	static void GetFilesNoRecurseEx(const char * path, const char * wildCard, Array<FileInfo> * results, bool includeDir = false);
};

//------------------------------------------------------------------------------
#endif // FOUNDATION_FILEIO_FILEIO_H
