
#include "debug.h"

#include <cstdlib>
#include <cstdio>
#include <cstdarg>
#include <cstring>

#ifdef WIN32
#include <Windows.h>
#endif

namespace loco
{
	void assert_failed(const char* expr, const char* file, int line, const char* format, ...)
	{
		va_list args;
		va_start(args, format);


		char msg_buffer[512];

		sprintf_s(msg_buffer, sizeof(msg_buffer), "%s(%d) : Assertion failed ( %s )\n", file, line, expr);
		vsprintf_s(msg_buffer + strlen(msg_buffer), sizeof(msg_buffer) - strlen(msg_buffer), format, args);

		va_end(args);

		// Display error window
		#ifdef WIN32
		MessageBox(NULL, msg_buffer, "Loco Engine Error", MB_OK | MB_ICONERROR);
		#endif

		// Exit
		exit(1);
	}
}
