// PathUtils.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/FileIO/PathUtils.h"

// Exists
//------------------------------------------------------------------------------
/*static*/ bool PathUtils::IsFolderPath(const AString & path)
{
	size_t pathLen = path.GetLength();
	if (pathLen > 0)
	{
		const char lastChar = path[pathLen - 1];

		// Handle both slash types so we cope with non-cleaned paths
		if ((lastChar == NATIVE_SLASH) || (lastChar == OTHER_SLASH))
		{
			return true;
		}
	}
	return false;
}

// IsfullPath
//------------------------------------------------------------------------------
/*static*/ bool PathUtils::IsFullPath(const AString & path)
{
#if defined(__WINDOWS__)
	// full paths on Windows are in X: format
	if (path.GetLength() >= 2)
	{
		return (path[1] == ':');
	}
	return false;
#elif defined(__LINUX__) || defined(__APPLE__)
	// full paths on Linux/OSX/IOS begin with a slash
	return path.BeginsWith(NATIVE_SLASH);
#endif
}

// ArePathsEqual
//------------------------------------------------------------------------------
/*static*/ bool PathUtils::ArePathsEqual(const AString & cleanPathA, const AString & cleanPathB)
{
#if defined(__LINUX__) || defined(__IOS__)
	// Case Sensitive
	return (cleanPathA == cleanPathB);
#endif

#if defined(__WINDOWS__) || defined(__OSX__)
	// Case Insensitive
	return (cleanPathA.CompareI(cleanPathB) == 0);
#endif
}

// IsWildcardMatch
//------------------------------------------------------------------------------
/*static*/ bool PathUtils::IsWildcardMatch(const char * pattern, const char * path)
{
#if defined(__LINUX__)
	// Linux : Case sensitive
	return AString::Match(pattern, path);
#else
	// Windows & OSX : Case insensitive
	return AString::MatchI(pattern, path);
#endif
}

// PathBeginsWith
//------------------------------------------------------------------------------
/*static*/ bool PathUtils::PathBeginsWith(const AString & cleanPath, const AString & cleanSubPath)
{
#if defined(__LINUX__)
	// Linux : Case sensitive
	return cleanPath.BeginsWith(cleanSubPath);
#else
	// Windows & OSX : Case insensitive
	return cleanPath.BeginsWithI(cleanSubPath);
#endif
}

// PathEndsWithFile
//------------------------------------------------------------------------------
/*static*/ bool PathUtils::PathEndsWithFile(const AString & cleanPath, const AString & fileName)
{
	// Work out if ends match
#if defined(__LINUX__)
	// Linux : Case sensitive
	bool endMatch = cleanPath.EndsWith(fileName);
#else
	// Windows & OSX : Case insensitive
	bool endMatch = cleanPath.EndsWithI(fileName);
#endif
	if (!endMatch)
	{
		return false;
	}

	// if it's an entire match (a full path for example)
	if (cleanPath.GetLength() == fileName.GetLength())
	{
		return true;
	}

	// Sanity check - if fileName was longer then path (or equal) we can't get here
	ASSERT(cleanPath.GetLength() > fileName.GetLength());
	const size_t potentialSlashIndex = (cleanPath.GetLength() - fileName.GetLength()) - 1; // -1 for char before match
	const char potentialSlash = cleanPath[potentialSlashIndex];
	if (potentialSlash == NATIVE_SLASH)
	{
		return true; // full filename part matches (e.g. c:\thing\stuff.cpp | stuff.cpp )
	}
	return false; // fileName is only a partial match (e.g. c:\thing\otherstuff.cpp | stuff.cpp )
}

// GetFolderPaths
//------------------------------------------------------------------------------
/*static*/ bool PathUtils::GetFileDirectory(const AString & file, AString & folderPath)
{
	const char * end = file.FindLast(NATIVE_SLASH);
	if (!end)
	{
		return false;
	}
	folderPath.Assign(file.Get(), end);
	return true;
}

// GetFileBaseName
//------------------------------------------------------------------------------
/*static*/ bool PathUtils::GetFileBaseName(const AStringView & file, AStringRef & baseName)
{
	const char * start = file.FindLast(NATIVE_SLASH);
	const char * end = file.GetEnd();
	if (!start)
	{
		baseName = file.Get();
		return true;
	}
	if (start == end)
	{
		return false;
	}
	baseName.Assign(start + 1, end);
	return true;
}

// GetFileBaseName
//------------------------------------------------------------------------------
/*static*/ AStringView PathUtils::GetFileBaseName(const AStringView & file)
{
	//AStringView baseName(file);
	const char * start = file.FindLast(NATIVE_SLASH);
	const char * end = file.GetEnd();
	if (!start)
	{
		return AStringView(file);
	}
	if (start == end)
	{
		return false;
	}
	return AStringView(start + 1, end);
}

// GetPathWithoutExt
//------------------------------------------------------------------------------
/*static*/ bool PathUtils::GetPathWithoutExt(const AString & path, AString & pathWithoutExt)
{
	const char * end = path.FindLast('.');
	if (!end)
	{
		pathWithoutExt = path;
		return false;
	}
	pathWithoutExt.Assign(path.Get(), end);
	return true;
}

// GetRelativePath
//------------------------------------------------------------------------------
/*static*/ bool PathUtils::GetRelativePath(const AString & path, const AString & relPath, AString & endPath)
{
	AStackString<> relDir(relPath);
	EnsureTrailingSlash((AStringRef)relDir);

	if (path.BeginsWith(relDir))
	{
		endPath = AString(path.Get() + relDir.GetLength(), path.GetEnd());
		return true;
	}
	else
	{
		endPath = path;
		return false;
	}
}

// EnsureTrailingSlash
//------------------------------------------------------------------------------
/*static*/ void PathUtils::EnsureTrailingSlash(AStringRef & path)
{
	// check for existing slash
	size_t pathLen = path.GetLength();
	if (pathLen > 0)
	{
		const char lastChar = path[pathLen - 1];
		if (lastChar == NATIVE_SLASH)
		{
			return; // good slash - nothing to do
		}
		if (lastChar == OTHER_SLASH)
		{
			// bad slash, do fixup
			path[pathLen - 1] = NATIVE_SLASH;
			return;
		}
	}

	// add slash
	path += NATIVE_SLASH;
}

// FixupFolderPath
//------------------------------------------------------------------------------
/*static*/ void PathUtils::FixupFolderPath(AStringRef & path)
{
	// Normalize slashes - TODO:C This could be optimized into one pass
	path.Replace(OTHER_SLASH, NATIVE_SLASH);
#if defined(__WINDOWS__)
	bool isUNCPath = path.BeginsWith(NATIVE_DOUBLE_SLASH);
#endif
	while (path.Replace(NATIVE_DOUBLE_SLASH, NATIVE_SLASH_STR)) {}

#if defined(__WINDOWS__)
	if (isUNCPath)
	{
		AStackString<> copy(path.Get());
		path.Clear();
		path += NATIVE_SLASH; // Restore double slash by adding one back
		path += copy;
	}
#endif

	// ensure slash termination
	if (path.EndsWith(NATIVE_SLASH) == false)
	{
		path += NATIVE_SLASH;
	}
}

// FixupFilePath
//------------------------------------------------------------------------------
/*static*/ void PathUtils::FixupFilePath(AStringRef & path)
{
	// Normalize slashes - TODO:C This could be optimized into one pass
	path.Replace(OTHER_SLASH, NATIVE_SLASH);
	while (path.Replace(NATIVE_DOUBLE_SLASH, NATIVE_SLASH_STR)) {}

	// Sanity check - calling this function on a folder path is an error
	ASSERT(path.EndsWith(NATIVE_SLASH) == false);
}

//------------------------------------------------------------------------------
