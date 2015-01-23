/*
 * Copyright 2010-2013 Branimir Karadzic. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-2-Clause
 */

#ifndef LOCO_TIMER_H_HEADER_GUARD
#define LOCO_TIMER_H_HEADER_GUARD

#include "platform.h"
#include "type.h"

#if LOCO_PLATFORM_ANDROID
#	include <time.h> // clock, clock_gettime
#elif LOCO_PLATFORM_WINDOWS || LOCO_PLATFORM_WINRT
#	include <windows.h>
#else
#	include <sys/time.h> // gettimeofday
#endif // LOCO_PLATFORM_

namespace loco
{
	inline int64 hp_counter()
	{
#if LOCO_PLATFORM_WINDOWS || LOCO_PLATFORM_WINRT
		LARGE_INTEGER li;
		// Performance counter value may unexpectedly leap forward
		// http://support.microsoft.com/kb/274323
		QueryPerformanceCounter(&li);
		int64 i64 = li.QuadPart;
#elif LOCO_PLATFORM_ANDROID
		struct timespec now;
		clock_gettime(CLOCK_MONOTONIC, &now);
		int64 i64 = now.tv_sec*INT64_C(1000000000) + now.tv_nsec;
#else
		struct timeval now;
		gettimeofday(&now, 0);
		int64 i64 = now.tv_sec*INT64_C(1000000) + now.tv_usec;
#endif // LOCO_PLATFORM_
		return i64;
	}

	inline int64 hp_frequency()
	{
#if LOCO_PLATFORM_WINDOWS || LOCO_PLATFORM_XBOX360 || LOCO_PLATFORM_WINRT
		LARGE_INTEGER li;
		QueryPerformanceFrequency(&li);
		return li.QuadPart;
#elif LOCO_PLATFORM_ANDROID
		return INT64_C(1000000000);
#else
		return INT64_C(1000000);
#endif // LOCO_PLATFORM_
	}

} // namespace loco

#endif // LOCO_TIMER_H_HEADER_GUARD
