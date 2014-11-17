/*
 * Copyright 2011-2014 Branimir Karadzic. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-2-Clause
 */

#include "entry_p.h"

#if BX_PLATFORM_EMSCRIPTEN

#include <emscripten.h>

namespace loco
{
	namespace entry
	{
		const Event* poll()
		{
			return NULL;
		}

		const Event* poll(WindowHandle _handle)
		{
			BX_UNUSED(_handle);
			return NULL;
		}

		void release(const Event* _event)
		{
			BX_UNUSED(_event);
		}

		WindowHandle create_window(int32_t _x, int32_t _y, uint32_t _width, uint32_t _height, uint32_t _flags, const char* _title)
		{
			BX_UNUSED(_x, _y, _width, _height, _flags, _title);
			WindowHandle handle = { UINT16_MAX };
			return handle;
		}

		void destroy_window(WindowHandle _handle)
		{
			BX_UNUSED(_handle);
		}

		void set_window_pos(WindowHandle _handle, int32_t _x, int32_t _y)
		{
			BX_UNUSED(_handle, _x, _y);
		}

		void set_window_size(WindowHandle _handle, uint32_t _width, uint32_t _height)
		{
			BX_UNUSED(_handle, _width, _height);
		}

		void set_window_title(WindowHandle _handle, const char* _title)
		{
			BX_UNUSED(_handle, _title);
		}

		void toggle_window_frame(WindowHandle _handle)
		{
			BX_UNUSED(_handle);
		}

		void set_mouse_lock(WindowHandle _handle, bool _lock)
		{
			BX_UNUSED(_handle, _lock);
		}
	} // namespace entry
} // namespace loco

int main(int _argc, char** _argv)
{
	return loco::entry::main(_argc, _argv);
}

#endif // BX_PLATFORM_EMSCRIPTEN
