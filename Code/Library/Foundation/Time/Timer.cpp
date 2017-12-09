// Timer.cpp
//------------------------------------------------------------------------------

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Time/Timer.h"
#include "Foundation/Env/Assert.h"

// System
#if defined(__WINDOWS__)
	#include <Windows.h>
#elif defined(__APPLE__)
	#include <mach/mach.h>
	#include <mach/mach_time.h>
#endif

// Static
//------------------------------------------------------------------------------
int64 Timer::s_Frequency = 0;
float Timer::s_FrequencyInvFloat = 0.0f;
float Timer::s_FrequencyInvFloatMS = 0.0f;

// helper to initialize frequency at startup
class GlobalTimerFrequencyInitializer
{
public:
	GlobalTimerFrequencyInitializer()
	{
#if defined(__WINDOWS__)
		ASSERT(Timer::s_Frequency == 0);
		LARGE_INTEGER freq;
		VERIFY(QueryPerformanceFrequency(&freq));
		Timer::s_Frequency = freq.QuadPart;
#elif defined(__APPLE__)
		mach_timebase_info_data_t info;
		mach_timebase_info(&info);
		Timer::s_Frequency = (int64_t)(info.numer / info.denom) * 1000000000;
#else
		Timer::s_Frequency = 1000000000ULL;
#endif
		Timer::s_FrequencyInvFloat = (float)(1.0 / (double)Timer::s_Frequency);
		Timer::s_FrequencyInvFloatMS = (float)(1.0 / (double)Timer::s_Frequency * 1000.0);
	}
};
GlobalTimerFrequencyInitializer g_GlobalTimerFrequencyInitializer;

// GetNow
//------------------------------------------------------------------------------
int64 Timer::GetNow()
{
#if defined(__WINDOWS__)
	ASSERT(s_Frequency);
	LARGE_INTEGER now;
	VERIFY(QueryPerformanceCounter(&now));
	return now.QuadPart;
#elif defined(__APPLE__)
	return (int64_t)mach_absolute_time();
#else
	#error Unknown platform
#endif
}

//------------------------------------------------------------------------------
