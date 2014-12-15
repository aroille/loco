
#include "debug.h"
#include "loco.h"

#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <cstring>

#ifdef WIN32
#include <Windows.h>
#endif

namespace loco
{
	void assert_failed(const char* expr, const char* module, const char* file, int line, const char* format, ...)
	{
		char msg_buffer[512];

		va_list args;
		va_start(args, format);

		// log
		sprintf_s(msg_buffer, sizeof(msg_buffer), "ASSERT %s(%d) : %s", file, line, format);
		log.log(Log::Fatal, module, msg_buffer, args);
		
		// popup message
		sprintf_s(msg_buffer, sizeof(msg_buffer), "%s(%d) \n", file, line);
		vsprintf_s(msg_buffer + strlen(msg_buffer), sizeof(msg_buffer) - strlen(msg_buffer), format, args);

		char message_title[512];
		sprintf_s(message_title, sizeof(message_title), "%s :: Assertion Failed", module);

		#ifdef WIN32
		MessageBox(NULL, msg_buffer, message_title, MB_OK | MB_ICONERROR);
		#endif

		va_end(args);

		// Exit
		exit(1);
	}
}
