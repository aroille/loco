#ifndef LOG_H_HEADER_GUARD
#define LOG_H_HEADER_GUARD

#include <cstdarg>

#define LOCO_LOG_DEBUG(module, msg, ...)		{ loco::Log::instance.debug(module, msg, ##__VA_ARGS__); }
#define LOCO_LOG_INFO(module, msg, ...)			{ loco::Log::instance.info(module, msg, ##__VA_ARGS__); }
#define LOCO_LOG_WARNING(module, msg, ...)	{ loco::Log::instance.warning(module, msg, ##__VA_ARGS__); }
#define LOCO_LOG_ERROR(module, msg, ...)		{ loco::Log::instance.error(module, msg, ##__VA_ARGS__); }
#define LOCO_LOG_FATAL(module, msg, ...)		{ loco::Log::instance.fatal(module, msg, ##__VA_ARGS__); }
#define LOCO_LOG(level, module, msg, ...)		{ loco::Log::instance.log(level, module, msg, ##__VA_ARGS__); }

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

		void debug(const char* module, const char* msg = "", ...) const;
		void info(const char* module, const char* msg = "", ...) const;
		void warning(const char* module, const char* msg = "", ...) const;
		void error(const char* module, const char* msg = "", ...) const;
		void fatal(const char* module, const char* msg = "", ...) const;

		void log(Level level, const char* module, const char* msg, va_list args) const;

		static const Log instance;
	};
}

#endif // LOG_H_HEADER_GUARD

