/*
* Copyright 2010-2013 Branimir Karadzic. All rights reserved.
* License: http://www.opensource.org/licenses/BSD-2-Clause
*/

#ifndef PLATFORM_H_HEADER_GUARD
#define PLATFORM_H_HEADER_GUARD

#define LOCO_COMPILER_CLANG           0
#define LOCO_COMPILER_CLANG_ANALYZER  0
#define LOCO_COMPILER_GCC             0
#define LOCO_COMPILER_MSVC            0
#define LOCO_COMPILER_MSVC_COMPATIBLE 0

#define LOCO_CPU_ARM  0
#define LOCO_CPU_PPC  0
#define LOCO_CPU_X86  0

#define LOCO_ARCH_64BIT 0
#define LOCO_ARCH_32BIT 0

#define LOCO_CPU_ENDIAN_BIG    0
#define LOCO_CPU_ENDIAN_LITTLE 0

#define LOCO_PLATFORM_ANDROID    0
#define LOCO_PLATFORM_IOS        0
#define LOCO_PLATFORM_LINUX      0
#define LOCO_PLATFORM_OSX        0
#define LOCO_PLATFORM_WINDOWS    0
#define LOCO_PLATFORM_WINRT      0



// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Compilers
#if defined(__clang__)
#	undef  LOCO_COMPILER_CLANG
#	define LOCO_COMPILER_CLANG (__clang_major__ * 10000 + __clang_minor__ * 100 + __clang_patchlevel__)
#	if defined(__clang_analyzer__)
#		undef  LOCO_COMPILER_CLANG_ANALYZER
#		define LOCO_COMPILER_CLANG_ANALYZER 1
#	endif
#	if defined(_MSC_VER)
#		undef  LOCO_COMPILER_MSVC_COMPATIBLE
#		define LOCO_COMPILER_MSVC_COMPATIBLE _MSC_VER
#	endif
#elif defined(_MSC_VER)
#	undef LOCO_COMPILER_MSVC
#	define LOCO_COMPILER_MSVC _MSC_VER
#	undef LOCO_COMPILER_MSVC_COMPATIBLE
#	define LOCO_COMPILER_MSVC_COMPATIBLE _MSC_VER
#elif defined(__GNUC__)
#	undef LOCO_COMPILER_GCC
#	define LOCO_COMPILER_GCC (__GNUC__ * 10000 + __GNUC_MINOR__ * 100 + __GNUC_PATCHLEVEL__)
#else
#	error "LOCO_COMPILER_ is not defined!"
#endif //

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Architectures
#if defined(__arm__) || defined(_M_ARM)
#	undef  LOCO_CPU_ARM
#	define LOCO_CPU_ARM 1
#	define LOCO_CACHE_LINE_SIZE 64
#elif defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__)
#	undef  LOCO_CPU_PPC
#	define LOCO_CPU_PPC 1
#	define LOCO_CACHE_LINE_SIZE 128
#elif defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
#	undef  LOCO_CPU_X86
#	define LOCO_CPU_X86 1
#	define LOCO_CACHE_LINE_SIZE 64
#else
#	error "LOCO_CPU_ is not defined!"
#endif //

#if defined(__x86_64__) || defined(_M_X64) || defined(__64BIT__) || defined(__powerpc64__) || defined(__ppc64__)
#	undef  LOCO_ARCH_64BIT
#	define LOCO_ARCH_64BIT 64
#else	   LOCO
#	undef  LOCO_ARCH_32BIT
#	define LOCO_ARCH_32BIT 32
#endif //

#if LOCO_CPU_PPC
#	undef  LOCO_CPU_ENDIAN_BIG
#	define LOCO_CPU_ENDIAN_BIG 1
#else	   LOCO
#	undef  LOCO_CPU_ENDIAN_LITTLE
#	define LOCO_CPU_ENDIAN_LITTLE 1
#endif //

// http://sourceforge.net/apps/mediawiki/predef/index.php?title=Operating_Systems
#if defined(_WIN32) || defined(_WIN64)
// http://msdn.microsoft.com/en-us/library/6sehtctf.aspx
#	if !defined(WINAPI_FAMILY) || (WINAPI_FAMILY == WINAPI_FAMILY_DESKTOP_APP)
#		undef  LOCO_PLATFORM_WINDOWS
#		if !defined(WINVER) && !defined(_WIN32_WINNT)
#			if LOCO_ARCH_64BIT
//				When building 64-bit target Win7 and above.
#				define WINVER 0x0601
#				define _WIN32_WINNT 0x0601
#			else
//				Windows Server 2003 with SP1, Windows XP with SP2 and above
#				define WINVER 0x0502
#				define _WIN32_WINNT 0x0502
#			endif // LOCO_ARCH_64BIT
#		endif // !defined(WINVER) && !defined(_WIN32_WINNT)
#		define LOCO_PLATFORM_WINDOWS _WIN32_WINNT
#	else
#		undef  LOCO_PLATFORM_WINRT
#		define LOCO_PLATFORM_WINRT 1
#	endif
#elif defined(__ANDROID__)
// Android compiler defines __linux__
#	include <android/api-level.h>
#	undef  LOCO_PLATFORM_ANDROID
#	define LOCO_PLATFORM_ANDROID __ANDROID_API__
#elif defined(__linux__)
#	undef  LOCO_PLATFORM_LINUX
#	define LOCO_PLATFORM_LINUX 1
#elif defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
#	undef  LOCO_PLATFORM_IOS
#	define LOCO_PLATFORM_IOS 1
#elif defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
#	undef  LOCO_PLATFORM_OSX
#	define LOCO_PLATFORM_OSX 1
#else
#	error "LOCO_PLATFORM_* is not defined!"
#endif //


#if LOCO_COMPILER_GCC
#	define LOCO_COMPILER_NAME "GCC " \
				#"." \
				#"." \
				#__GNUC_PATCHLEVEL__
#elif LOCO_COMPILER_CLANG
#	define LOCO_COMPILER_NAME "Clang " \
				#__clang_major__ "." \
				#__clang_minor__ "." \
				#__clang_patchlevel__
#elif LOCO_COMPILER_MSVC
#	if LOCO_COMPILER_MSVC >= 1800
#		define LOCO_COMPILER_NAME "MSVC 12.0"
#	elif LOCO_COMPILER_MSVC >= 1700
#		define LOCO_COMPILER_NAME "MSVC 11.0"
#	elif LOCO_COMPILER_MSVC >= 1600
#		define LOCO_COMPILER_NAME "MSVC 10.0"
#	elif LOCO_COMPILER_MSVC >= 1500
#		define LOCO_COMPILER_NAME "MSVC 9.0"
#	else
#		define LOCO_COMPILER_NAME "MSVC"
#	endif //
#endif // LOCO_COMPILER_


#if LOCO_PLATFORM_ANDROID
#	define LOCO_PLATFORM_NAME "Android " \
				LOCO_STRINGIZE(LOCO_PLATFORM_ANDROID)
#elif LOCO_PLATFORM_IOS
#	define LOCO_PLATFORM_NAME "iOS"
#elif LOCO_PLATFORM_LINUX
#	define LOCO_PLATFORM_NAME "Linux"
#elif LOCO_PLATFORM_OSX
#	define LOCO_PLATFORM_NAME "OSX"
#elif LOCO_PLATFORM_WINDOWS
#	define LOCO_PLATFORM_NAME "Windows"
#elif LOCO_PLATFORM_WINRT
#	define LOCO_PLATFORM_NAME "WinRT"
#endif // LOCO_PLATFORM_

#if LOCO_CPU_ARM
#	define LOCO_CPU_NAME "ARM"
#elif LOCO_CPU_PPC
#	define LOCO_CPU_NAME "PowerPC"
#elif LOCO_CPU_X86
#	define LOCO_CPU_NAME "x86"
#endif // LOCO_CPU_

#if LOCO_ARCH_32BIT
#	define LOCO_ARCH_NAME "32-bit"
#elif LOCO_ARCH_64BIT
#	define LOCO_ARCH_NAME "64-bit"
#endif // LOCO_ARCH_


#ifdef LOCO_PLATFORM_WINDOWS
#	include "windows.h"
#endif

#endif // PLATFORM_H_HEADER_GUARD

