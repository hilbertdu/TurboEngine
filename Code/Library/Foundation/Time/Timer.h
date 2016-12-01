// Timer.h
//------------------------------------------------------------------------------
#pragma once
#ifndef CORE_TIME_TIMER_H
#define CORE_TIME_TIMER_H

// Includes
//------------------------------------------------------------------------------
#include "Foundation/Platform/Platform.h"
#include "Foundation/Platform/Types.h"

// Timer
//------------------------------------------------------------------------------
class Timer
{
public:
	Timer() { Start(); }
	inline ~Timer() {}

	inline void Start() { m_StartTime = GetNow(); }
	inline void Start(float time) { m_StartTime = GetNow() - (int64)((double)GetFrequency() * (double)time); }

	float GetElapsed() const
	{
		int64 now = GetNow();
		return ((float)(now - m_StartTime) * GetFrequencyInvFloat());
	}

	float GetElapsedMS() const
	{
		int64 now = GetNow();
		return ((float)(now - m_StartTime) * GetFrequencyInvFloatMS());
	}

	static int64 GetNow();
	static inline int64 GetFrequency() { return s_Frequency; }
	static inline float GetFrequencyInvFloat() { return s_FrequencyInvFloat; }
	static inline float GetFrequencyInvFloatMS() { return s_FrequencyInvFloatMS; }

private:
	int64 m_StartTime;

	// Frequency
	friend class GlobalTimerFrequencyInitializer;
	static int64 s_Frequency;
	static float s_FrequencyInvFloat;
	static float s_FrequencyInvFloatMS;
};

//------------------------------------------------------------------------------
#endif // CORE_TIME_TIMER_H
