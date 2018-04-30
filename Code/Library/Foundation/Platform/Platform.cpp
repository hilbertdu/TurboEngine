// Platform.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Config/Config.h"
#include "Foundation/Platform/Platform.h"
#include <codecvt>

#if defined(__WINDOWS__)
	#include <windows.h>
#else
	#error
#endif


// LoadLibrary
//------------------------------------------------------------------------------
/*static*/ void* Platform::LoadDynamicLibrary(const char * name, const char * directory)
{
#if defined(__WINDOWS__)
	std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> str_converter;
	::AddDllDirectory(str_converter.from_bytes(directory).c_str());
	return ::LoadLibraryExA(name, nullptr, LOAD_LIBRARY_SEARCH_DEFAULT_DIRS);
#else
	#error
#endif // __WINDOWS__
}

// GetLastErrorNo
//------------------------------------------------------------------------------
/*static*/ int Platform::GetLastErrorNo()
{
#if defined(__WINDOWS__)
	return ::GetLastError();
#else
	#error
#endif
}

//------------------------------------------------------------------------------