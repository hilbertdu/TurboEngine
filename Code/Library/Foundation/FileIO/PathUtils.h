// PathUtils.h
//------------------------------------------------------------------------------
#pragma once
#ifndef FOUNDATION_FILEIO_PATHUTILS_H
#define FOUNDATION_FILEIO_PATHUTILS_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Config/Config.h"
#include "Foundation/String/String.h"
#include "Foundation/String/StringView.h"

// Defines
//------------------------------------------------------------------------------
#if defined(__WINDOWS__)
	#define NATIVE_SLASH ('\\')
	#define NATIVE_SLASH_STR ("\\")
	#define NATIVE_DOUBLE_SLASH ("\\\\")
	#define OTHER_SLASH ('/')
#elif defined(__LINUX__) || defined(__APPLE__)
	#define NATIVE_SLASH ('/')
	#define NATIVE_SLASH_STR ("/")
	#define NATIVE_DOUBLE_SLASH ("//")
	#define OTHER_SLASH ('\\')
#endif

// For places that explicitly need slashes a certain way
// use these defines to signify the intent
#define BACK_SLASH ('\\')
#define FORWARD_SLASH ('/')

// FileIO
//------------------------------------------------------------------------------
class PathUtils
{
public:
	// Query Helpers
	//--------------
	static bool IsFolderPath(const AString & path);
	static bool IsFullPath(const AString & path);
	static bool ArePathsEqual(const AString & cleanPathA, const AString & cleanPathB);
	static bool IsWildcardMatch(const char * pattern, const char * path);
	static bool PathBeginsWith(const AString & cleanPath, const AString & cleanSubPath);
	static bool PathEndsWithFile(const AString & cleanPath, const AString & fileName);

	// Cleanup Helpers
	//----------------
	static void EnsureTrailingSlash(AStringRef & path);
	static void FixupFolderPath(AStringRef & path);
	static void FixupFilePath(AStringRef & path);

	// Finder Helpers
	//----------------
	static bool GetFileDirectory(const AString & file, AString & folderPath);
	static bool GetFileBaseName(const AStringView & path, AStringRef & baseName);
	static AStringView GetFileBaseName(const AStringView & file);

	static bool GetPathWithoutExt(const AString & path, AString & pathWithoutExt);
	static bool GetRelativePath(const AString & path, const AString & relPath, AString & endPath);
};

//------------------------------------------------------------------------------
#endif // FOUNDATION_FILEIO_PATHUTILS_H
