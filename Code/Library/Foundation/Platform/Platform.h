// Platform.h
//------------------------------------------------------------------------------

#pragma once
#ifndef FOUNDATION_PLATFORM_PLATFORM_H
#define FOUNDATION_PLATFORM_PLATFORM_H

class Platform
{
public:
	static void* LoadDynamicLibrary(const char * name, const char * directory = nullptr);
	static int	 GetLastErrorNo();
};

//------------------------------------------------------------------------------
#endif // FOUNDATION_PLATFORM_PLATFORM_H