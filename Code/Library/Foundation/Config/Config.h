// Config.h
//------------------------------------------------------------------------------

#pragma once
#ifndef FOUNDATION_CONFIG_CONFIG_H
#define FOUNDATION_CONFIG_CONFIG_H

#ifdef _WIN32
	#define __WIN32__
	#define __WINDOWS__
#endif

#ifdef _WIN64
	#define __WIN32__
	#define __WIN64__
	#define __WINDOWS__
#endif

#ifdef __WINDOWS__
	#if defined(_DEBUG)
		#define __DEBUG__
	#else
		#define __RELEASE__
	#endif
#endif

//------------------------------------------------------------------------------
#endif // FOUNDATION_CONFIG_CONFIG_H