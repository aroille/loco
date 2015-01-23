
#include "log.h"
#include "platform.h"

#include <ctime>
#include <cstdlib>
#include <cstdio>
#include <cstring>

namespace loco
{
	char LogLevelString[Log::Level::Count][8] = 
	{
		"DEBUG", 
		"INFO", 
		"WARNING", 
		"ERROR",
		"FATAL"
	};

	void Log::debug(const char* module, const char* msg, ...)
	{
		va_list args;
		va_start(args, msg);
		log(Level::Debug, module, msg, args);
		va_end(args);
	}

	void Log::info(const char* module, const char* msg, ...)
	{
		va_list args;
		va_start(args, msg);
		log(Level::Info, module, msg, args);
		va_end(args);
	}

	void Log::warning(const char* module, const char* msg, ...)
	{
		va_list args;
		va_start(args, msg);
		log(Level::Warning, module, msg, args);
		va_end(args);
	}

	void Log::error(const char* module, const char* msg, ...)
	{
		va_list args;
		va_start(args, msg);
		log(Level::Error, module, msg, args);
		va_end(args);
	}

	void Log::log(Level level, const char* module, const char* msg, va_list args)
	{
		// get time
		time_t t = time(0);
		tm* now = localtime(&t);

		// output format
		char msg_buffer[512];

		sprintf_s(msg_buffer, sizeof(msg_buffer), "  %-8s  %02d-%02d-%04d %02d:%02d:%02d  %-15s  ", 
			LogLevelString[(unsigned)level],
			now->tm_mday, now->tm_mon+1, now->tm_year+1900,
			now->tm_hour, now->tm_min, now->tm_sec,
			module);
		vsprintf_s(msg_buffer + strlen(msg_buffer), sizeof(msg_buffer)-strlen(msg_buffer), msg, args);
		
		// output log message
		printf(msg_buffer);
		printf("\n");

#ifdef LOCO_PLATFORM_WINDOWS
		OutputDebugStringA(msg_buffer);
		OutputDebugStringA("\n");
#endif
	}
}
