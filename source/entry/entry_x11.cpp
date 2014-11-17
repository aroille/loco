/*
 * Copyright 2011-2014 Branimir Karadzic. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-2-Clause
 */

#include "entry_p.h"

#if ENTRY_CONFIG_USE_NATIVE && (BX_PLATFORM_FREEBSD || BX_PLATFORM_LINUX || BX_PLATFORM_RPI)

#define XK_MISCELLANY
#define XK_LATIN1
#include <X11/keysymdef.h>
#include <bgfxplatform.h> // will include X11 which #defines None... Don't mess with order of includes.

#undef None
#include <bx/thread.h>
#include <bx/os.h>
#include <bx/handlealloc.h>
#include <string.h> // memset
#include <string>

namespace loco
{
namespace entry
{
	static uint8_t s_translateKey[512];

	static void initTranslateKey(uint16_t _xk, Key::Enum _key)
	{
		_xk += 256;
		BX_CHECK(_xk < BX_COUNTOF(s_translateKey), "Out of bounds %d.", _xk);
		s_translateKey[_xk&0x1ff] = (uint8_t)_key;
	}

	Key::Enum fromXk(uint16_t _xk)
	{
		_xk += 256;
		return 512 > _xk ? (Key::Enum)s_translateKey[_xk] : Key::None;
	}

	struct MainThreadEntry
	{
		int m_argc;
		char** m_argv;

		static int32_t threadFunc(void* _userData);
	};

	struct Msg
	{
		Msg()
			: m_x(0)
			, m_y(0)
			, m_width(0)
			, m_height(0)
			, m_flags(0)
		{
		}

		int32_t  m_x;
		int32_t  m_y;
		uint32_t m_width;
		uint32_t m_height;
		uint32_t m_flags;
		std::string m_title;
	};

	struct Context
	{
		Context()
			: m_modifiers(Modifier::None)
			, m_exit(false)
		{
			memset(s_translateKey, 0, sizeof(s_translateKey) );
			initTranslateKey(XK_Escape,       Key::Esc);
			initTranslateKey(XK_Return,       Key::Return);
			initTranslateKey(XK_Tab,          Key::Tab);
			initTranslateKey(XK_BackSpace,    Key::Backspace);
			initTranslateKey(XK_space,        Key::Space);
			initTranslateKey(XK_Up,           Key::Up);
			initTranslateKey(XK_Down,         Key::Down);
			initTranslateKey(XK_Left,         Key::Left);
			initTranslateKey(XK_Right,        Key::Right);
			initTranslateKey(XK_Page_Up,      Key::PageUp);
			initTranslateKey(XK_Page_Down,    Key::PageUp);
			initTranslateKey(XK_Home,         Key::Home);
			initTranslateKey(XK_KP_End,       Key::End);
			initTranslateKey(XK_Print,        Key::Print);
			initTranslateKey(XK_equal,        Key::Plus);
			initTranslateKey(XK_minus,        Key::Minus);
			initTranslateKey(XK_F1,           Key::F1);
			initTranslateKey(XK_F2,           Key::F2);
			initTranslateKey(XK_F3,           Key::F3);
			initTranslateKey(XK_F4,           Key::F4);
			initTranslateKey(XK_F5,           Key::F5);
			initTranslateKey(XK_F6,           Key::F6);
			initTranslateKey(XK_F7,           Key::F7);
			initTranslateKey(XK_F8,           Key::F8);
			initTranslateKey(XK_F9,           Key::F9);
			initTranslateKey(XK_F10,          Key::F10);
			initTranslateKey(XK_F11,          Key::F11);
			initTranslateKey(XK_F12,          Key::F12);
			initTranslateKey(XK_KP_Insert,    Key::NumPad0);
			initTranslateKey(XK_KP_End,       Key::NumPad1);
			initTranslateKey(XK_KP_Down,      Key::NumPad2);
			initTranslateKey(XK_KP_Page_Down, Key::NumPad3);
			initTranslateKey(XK_KP_Left,      Key::NumPad4);
			initTranslateKey(XK_KP_Begin,     Key::NumPad5);
			initTranslateKey(XK_KP_Right,     Key::NumPad6);
			initTranslateKey(XK_KP_Home,      Key::NumPad7);
			initTranslateKey(XK_KP_Up,        Key::NumPad8);
			initTranslateKey(XK_KP_Page_Up,   Key::NumPad9);
			initTranslateKey('0',             Key::Key0);
			initTranslateKey('1',             Key::Key1);
			initTranslateKey('2',             Key::Key2);
			initTranslateKey('3',             Key::Key3);
			initTranslateKey('4',             Key::Key4);
			initTranslateKey('5',             Key::Key5);
			initTranslateKey('6',             Key::Key6);
			initTranslateKey('7',             Key::Key7);
			initTranslateKey('8',             Key::Key8);
			initTranslateKey('9',             Key::Key9);
			initTranslateKey('a',             Key::KeyA);
			initTranslateKey('b',             Key::KeyB);
			initTranslateKey('c',             Key::KeyC);
			initTranslateKey('d',             Key::KeyD);
			initTranslateKey('e',             Key::KeyE);
			initTranslateKey('f',             Key::KeyF);
			initTranslateKey('g',             Key::KeyG);
			initTranslateKey('h',             Key::KeyH);
			initTranslateKey('i',             Key::KeyI);
			initTranslateKey('j',             Key::KeyJ);
			initTranslateKey('k',             Key::KeyK);
			initTranslateKey('l',             Key::KeyL);
			initTranslateKey('m',             Key::KeyM);
			initTranslateKey('n',             Key::KeyN);
			initTranslateKey('o',             Key::KeyO);
			initTranslateKey('p',             Key::KeyP);
			initTranslateKey('q',             Key::KeyQ);
			initTranslateKey('r',             Key::KeyR);
			initTranslateKey('s',             Key::KeyS);
			initTranslateKey('t',             Key::KeyT);
			initTranslateKey('u',             Key::KeyU);
			initTranslateKey('v',             Key::KeyV);
			initTranslateKey('w',             Key::KeyW);
			initTranslateKey('x',             Key::KeyX);
			initTranslateKey('y',             Key::KeyY);
			initTranslateKey('z',             Key::KeyZ);
		}

		int32_t run(int _argc, char** _argv)
		{
			XInitThreads();
			m_display = XOpenDisplay(0);

			int32_t screen = DefaultScreen(m_display);
			m_depth  = DefaultDepth(m_display, screen);
			m_visual = DefaultVisual(m_display, screen);
			m_root   = RootWindow(m_display, screen);

			XSetWindowAttributes windowAttrs;
			memset(&windowAttrs, 0, sizeof(windowAttrs) );
			windowAttrs.background_pixmap = 0;
			windowAttrs.border_pixel = 0;
			windowAttrs.event_mask = 0
					| ButtonPressMask
					| ButtonReleaseMask
					| ExposureMask
					| KeyPressMask
					| KeyReleaseMask
					| PointerMotionMask
					| ResizeRedirectMask
					| StructureNotifyMask
					;

			m_windowAlloc.alloc();
			m_window[0] = XCreateWindow(m_display
									, m_root
									, 0, 0
									, ENTRY_DEFAULT_WIDTH, ENTRY_DEFAULT_HEIGHT, 0
									, m_depth
									, InputOutput
									, m_visual
									, CWBorderPixel|CWEventMask
									, &windowAttrs
									);

			// Clear window to black.
			XSetWindowAttributes attr;
			memset(&attr, 0, sizeof(attr) );
			XChangeWindowAttributes(m_display, m_window[0], CWBackPixel, &attr);

			const char* wmDeleteWindowName = "WM_DELETE_WINDOW";
			Atom wmDeleteWindow;
			XInternAtoms(m_display, (char **)&wmDeleteWindowName, 1, False, &wmDeleteWindow);
			XSetWMProtocols(m_display, m_window[0], &wmDeleteWindow, 1);

			XMapWindow(m_display, m_window[0]);
			XStoreName(m_display, m_window[0], "BGFX");

			//
			bgfx::x11SetDisplayWindow(m_display, m_window[0]);

			MainThreadEntry mte;
			mte.m_argc = _argc;
			mte.m_argv = _argv;

			bx::Thread thread;
			thread.init(mte.threadFunc, &mte);

			WindowHandle defaultWindow = { 0 };
			m_eventQueue.postSizeEvent(defaultWindow, ENTRY_DEFAULT_WIDTH, ENTRY_DEFAULT_HEIGHT);

			while (!m_exit)
			{
				if (XPending(m_display) )
				{
					XEvent event;
					XNextEvent(m_display, &event);

					switch (event.type)
					{
						case Expose:
							break;

						case ConfigureNotify:
							break;

						case ClientMessage:
							if ( (Atom)event.xclient.data.l[0] == wmDeleteWindow)
							{
								m_eventQueue.postExitEvent();
							}
							break;

						case ButtonPress:
						case ButtonRelease:
							{
								const XButtonEvent& xbutton = event.xbutton;
								MouseButton::Enum mb;
								switch (xbutton.button)
								{
									case Button1: mb = MouseButton::Left;   break;
									case Button2: mb = MouseButton::Middle; break;
									case Button3: mb = MouseButton::Right;  break;
									default:      mb = MouseButton::None;   break;
								}

								if (MouseButton::None != mb)
								{
									WindowHandle handle = findHandle(xbutton.window);
									m_eventQueue.postMouseEvent(handle
										, xbutton.x
										, xbutton.y
										, 0
										, mb
										, event.type == ButtonPress
										);
								}
							}
							break;

						case MotionNotify:
							{
								const XMotionEvent& xmotion = event.xmotion;
								WindowHandle handle = findHandle(xmotion.window);
								m_eventQueue.postMouseEvent(handle
										, xmotion.x
										, xmotion.y
										, 0
										);
							}
							break;

						case KeyPress:
						case KeyRelease:
							{
								XKeyEvent& xkey = event.xkey;
								KeySym keysym = XLookupKeysym(&xkey, 0);
								switch (keysym)
								{
								case XK_Meta_L:    setModifier(Modifier::LeftMeta,   KeyPress == event.type); break;
								case XK_Meta_R:    setModifier(Modifier::RightMeta,  KeyPress == event.type); break;
								case XK_Control_L: setModifier(Modifier::LeftCtrl,   KeyPress == event.type); break;
								case XK_Control_R: setModifier(Modifier::RightCtrl,  KeyPress == event.type); break;
								case XK_Shift_L:   setModifier(Modifier::LeftShift,  KeyPress == event.type); break;
								case XK_Shift_R:   setModifier(Modifier::RightShift, KeyPress == event.type); break;
								case XK_Alt_L:     setModifier(Modifier::LeftAlt,    KeyPress == event.type); break;
								case XK_Alt_R:     setModifier(Modifier::RightAlt,   KeyPress == event.type); break;

								default:
									{
										Key::Enum key = fromXk(keysym);
										if (Key::None != key)
										{
											WindowHandle handle = findHandle(xkey.window);
											m_eventQueue.postKeyEvent(handle, key, m_modifiers, KeyPress == event.type);
										}
									}
									break;
								}
							}
							break;

						case ResizeRequest:
							{
								const XResizeRequestEvent& xresize = event.xresizerequest;
								XResizeWindow(xresize.display, xresize.window, xresize.width, xresize.height);
								WindowHandle handle = findHandle(xresize.window);
								if (is_valid(handle) )
								{
									m_eventQueue.postSizeEvent(handle, xresize.width, xresize.height);
								}
							}
							break;
					}
				}
			}

			thread.shutdown();

			XUnmapWindow(m_display, m_window[0]);
			XDestroyWindow(m_display, m_window[0]);

			return EXIT_SUCCESS;
		}

		void setModifier(Modifier::Enum _modifier, bool _set)
		{
			m_modifiers &= ~_modifier;
			m_modifiers |= _set ? _modifier : 0;
		}

		void create_window(WindowHandle _handle, Msg* msg)
		{
			XSetWindowAttributes windowAttrs;
			memset(&windowAttrs, 0, sizeof(windowAttrs) );
			windowAttrs.background_pixmap = 0;
			windowAttrs.border_pixel = 0;
			windowAttrs.event_mask = 0
					| ButtonPressMask
					| ButtonReleaseMask
					| ExposureMask
					| KeyPressMask
					| KeyReleaseMask
					| PointerMotionMask
					| ResizeRedirectMask
					| StructureNotifyMask
					;

			Window window = XCreateWindow(m_display
									, m_root
									, msg->m_x
									, msg->m_y
									, msg->m_width
									, msg->m_height
									, 0
									, m_depth
									, InputOutput
									, m_visual
									, CWBorderPixel|CWEventMask
									, &windowAttrs
									);
			m_window[_handle.idx] = window;

			// Clear window to black.
			XSetWindowAttributes attr;
			memset(&attr, 0, sizeof(attr) );
			XChangeWindowAttributes(m_display, window, CWBackPixel, &attr);

			const char* wmDeleteWindowName = "WM_DELETE_WINDOW";
			Atom wmDeleteWindow;
			XInternAtoms(m_display, (char **)&wmDeleteWindowName, 1, False, &wmDeleteWindow);
			XSetWMProtocols(m_display, window, &wmDeleteWindow, 1);

			XMapWindow(m_display, window);
			XStoreName(m_display, window, msg->m_title.c_str() );

			m_eventQueue.postSizeEvent(_handle, msg->m_width, msg->m_height);

			union cast
			{
				void* p;
				::Window w;
			};

			cast c;
			c.w = window;
			m_eventQueue.postWindowEvent(_handle, c.p);

			delete msg;
		}

		WindowHandle findHandle(Window _window)
		{
			bx::LwMutexScope scope(m_lock);
			for (uint32_t ii = 0, num = m_windowAlloc.getNumHandles(); ii < num; ++ii)
			{
				uint16_t idx = m_windowAlloc.getHandleAt(ii);
				if (_window == m_window[idx])
				{
					WindowHandle handle = { idx };
					return handle;
				}
			}

			WindowHandle invalid = { UINT16_MAX };
			return invalid;
		}

		uint8_t m_modifiers;
		bool m_exit;

		EventQueue m_eventQueue;
		bx::LwMutex m_lock;
		bx::HandleAllocT<ENTRY_CONFIG_MAX_WINDOWS> m_windowAlloc;

		int32_t m_depth;
		Visual* m_visual;
		Window  m_root;

		Display* m_display;
		Window m_window[ENTRY_CONFIG_MAX_WINDOWS];
		uint32_t m_flags[ENTRY_CONFIG_MAX_WINDOWS];
	};

	static Context s_ctx;

	int32_t MainThreadEntry::threadFunc(void* _userData)
	{
		MainThreadEntry* self = (MainThreadEntry*)_userData;
		int32_t result = main(self->m_argc, self->m_argv);
		s_ctx.m_exit = true;
		return result;
	}

	const Event* poll()
	{
		return s_ctx.m_eventQueue.poll();
	}

	const Event* poll(WindowHandle _handle)
	{
		return s_ctx.m_eventQueue.poll(_handle);
	}

	void release(const Event* _event)
	{
		s_ctx.m_eventQueue.release(_event);
	}

	WindowHandle create_window(int32_t _x, int32_t _y, uint32_t _width, uint32_t _height, uint32_t _flags, const char* _title)
	{
		bx::LwMutexScope scope(s_ctx.m_lock);
		WindowHandle handle = { s_ctx.m_windowAlloc.alloc() };

		if (is_valid(handle) )
		{
			Msg* msg = new Msg;
			msg->m_x      = _x;
			msg->m_y      = _y;
			msg->m_width  = _width;
			msg->m_height = _height;
			msg->m_title  = _title;
			msg->m_flags  = _flags;
			s_ctx.createWindow(handle, msg);
		}

		return handle;
	}

	void destroy_window(WindowHandle _handle)
	{
		if (is_valid(_handle) )
		{
			s_ctx.m_eventQueue.postWindowEvent(_handle, NULL);
			XUnmapWindow(s_ctx.m_display, s_ctx.m_window[_handle.idx]);
			XDestroyWindow(s_ctx.m_display, s_ctx.m_window[_handle.idx]);

			bx::LwMutexScope scope(s_ctx.m_lock);
			s_ctx.m_windowAlloc.free(_handle.idx);
		}
	}

	void set_window_pos(WindowHandle _handle, int32_t _x, int32_t _y)
	{
		BX_UNUSED(_handle, _x, _y);
	}

	void set_window_size(WindowHandle _handle, uint32_t _width, uint32_t _height)
	{
		BX_UNUSED(_handle);
		XResizeRequestEvent ev;
		ev.type = ResizeRequest;
		ev.serial = 0;
		ev.send_event = true;
		ev.display = s_ctx.m_display;
		ev.window = s_ctx.m_window[0];
		ev.width = (int)_width;
		ev.height = (int)_height;
		XSendEvent(s_ctx.m_display, s_ctx.m_window[0], false, ResizeRedirectMask, (XEvent*)&ev);
	}

	void set_window_title(WindowHandle _handle, const char* _title)
	{
		XStoreName(s_ctx.m_display, s_ctx.m_window[_handle.idx], _title);
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
	using namespace entry;
	return s_ctx.run(_argc, _argv);
}

#endif // ENTRY_CONFIG_USE_NATIVE && (BX_PLATFORM_FREEBSD || BX_PLATFORM_LINUX || BX_PLATFORM_RPI)
