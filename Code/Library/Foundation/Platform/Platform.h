// Platform.h
//------------------------------------------------------------------------------

#pragma once
#ifndef FOUNDATION_PLATFORM_PLATFORM_H
#define FOUNDATION_PLATFORM_PLATFORM_H

#ifdef _WIN32
	#define __WIN32__
	#define __WINDOWS__
#endif

#ifdef _WIN64
	#define __WIN32__
	#define __WIN64__
	#define __WINDOWS__
#endif

//------------------------------------------------------------------------------
#endif // FOUNDATION_PLATFORM_PLATFORM_H