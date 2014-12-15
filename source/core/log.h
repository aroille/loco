#ifndef LOG_H_HEADER_GUARD
#define LOG_H_HEADER_GUARD

#include <cstdarg>

namespace loco
{
	class Log
	{
	public:
		enum Level
		{
			Debug,
			Info,
			Warning,
			Error,
			Fatal,
			Count
		};

		void debug(const char* module, const char* msg = "", ...);
		void info(const char* module, const char* msg = "", ...);
		void warning(const char* module, const char* msg = "", ...);
		void error(const char* module, const char* msg = "", ...);

		void log(Level level, const char* module, const char* msg, va_list args);
	};
}

#endif // LOG_H_HEADER_GUARD

