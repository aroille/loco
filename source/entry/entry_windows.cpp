
#include "defines.h"
#include "entry.h"
#include "loco.h"
#include "timer.h"
#include "type.h"
#include "bgfxplatform.h"

#include <windows.h>
#include <windowsx.h>

#include <xinput.h>
#define global_variable static
#define internal static

using namespace loco;


global_variable bool		 global_running;
global_variable bool		 global_has_focus;
global_variable int32		 global_window_width;
global_variable int32		 global_window_height;
global_variable bool		 global_locked_mouse;
global_variable Vector2i global_locked_mouse_position;

// XInputGetState
#define X_INPUT_GET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_STATE* pState)
typedef X_INPUT_GET_STATE(x_input_get_state);
X_INPUT_GET_STATE(XInputGetStateStub)
{
	return 0;
}
global_variable x_input_get_state* XInputGetState_ = XInputGetStateStub;
#define XInputGetState XInputGetState_

// XInputSetState
#define X_INPUT_SET_STATE(name) DWORD WINAPI name(DWORD dwUserIndex, XINPUT_VIBRATION* pVibration)
typedef X_INPUT_SET_STATE(x_input_set_state);
X_INPUT_SET_STATE(XInputSetStateStub)
{
	return 0;
}
global_variable x_input_set_state* XInputSetState_ = XInputSetStateStub;
#define XInputSetState XInputSetState_

internal void 
win32_load_xinput()
{
	HMODULE x_input_library = LoadLibraryA("xinput1_3.dll");
	if (x_input_library)
	{
		XInputGetState_ = (x_input_get_state*)GetProcAddress(x_input_library, "XInputGetState");
		XInputSetState_ = (x_input_set_state*)GetProcAddress(x_input_library, "XInputSetState");
	}
}

internal void
win32_process_keyboard_message(GameButtonState* new_state, bool is_down)
{
	new_state->is_down = is_down;
	++new_state->transition_count;
}

internal void
win32_process_mouse_message(uint32 button_state, uint32 button_bit, GameButtonState* new_state)
{
	new_state->is_down = (button_state & button_bit) == button_bit;
	++new_state->transition_count;
}

internal void
win32_process_xinput_digital_buttons(DWORD button_state, DWORD button_bit, GameButtonState* old_state, GameButtonState* new_state)
{
	new_state->is_down = (button_state & button_bit) == button_bit;
	new_state->transition_count = (old_state->is_down != new_state->is_down) ? 1 : 0;
}

internal void
win32_process_xinput_analogic_stick(SHORT xinput_value, float* stick_value)
{
	if (xinput_value > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE || xinput_value < -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		*stick_value = xinput_value > 0 ? (xinput_value / 32767.0f) : (xinput_value / 32768.0f);
	}
	else
	{
		*stick_value = 0.0f;
	}
}

LRESULT CALLBACK 
MainWindowCallback(HWND window, UINT message, WPARAM w_param,	LPARAM l_param)
{
	LRESULT result = 0;
	switch (message)
	{
	case WM_DESTROY:
	{
		global_running = false;
	} break;

	case WM_CLOSE:
	{
		global_running = false;
	} break;

	case WM_SIZE:
	{
		OutputDebugStringA("WM_SIZE\n");
		global_window_width = LOWORD(l_param);
		global_window_height = HIWORD(l_param);
		if (loco::is_initialized())
			loco::renderer.reset(global_window_width, global_window_height);
	} break;

	case WM_ACTIVATEAPP:
	{
		global_has_focus = w_param;
		OutputDebugStringA("WM_ACTIVEAPP\n");
	} break;

	default:
	{
		result = DefWindowProc(window, message, w_param, l_param);
	} break;
	}

	return result;
}

internal void 
win32_process_pending_messages(GameControllerInput* keyboard_controller, MouseInput* mouse_controller)
{
	MSG message;
	while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
	{

		switch (message.message)
		{

		case WM_QUIT:
		{
			global_running = false;
		}
		break;

		case WM_LBUTTONUP:
		case WM_LBUTTONDOWN:
			win32_process_mouse_message(static_cast<uint32>(message.wParam), MK_LBUTTON, &mouse_controller->left);
			break;

		case WM_MBUTTONUP:
		case WM_RBUTTONUP:
			win32_process_mouse_message(static_cast<uint32>(message.wParam), MK_MBUTTON, &mouse_controller->middle);
			break;

		case WM_MBUTTONDOWN:
		case WM_RBUTTONDOWN:
			win32_process_mouse_message(static_cast<uint32>(message.wParam), MK_RBUTTON, &mouse_controller->right);
			break;

		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			uint32 vk_code = static_cast<uint32>(message.wParam);
			bool was_down = ((message.lParam & (1 << 30)) != 0);
			bool is_down = ((message.lParam & (1 << 31)) == 0);

			if (is_down != was_down)
			{
				if (vk_code == 'W')
				{
					//keyboard_controller->left_thumb.y += is_down ? 1.0f : -1.0f;
					keyboard_controller->left_thumb.y = is_down ? (keyboard_controller->left_thumb.y < 0.0f ? 0.0f : 1.0f) 
																											: (keyboard_controller->left_thumb.y > 0.0f ? 0.0f :-1.0f);
					//win32_process_keyboard_message(&keyboard_controller->up, is_down);
				}
				else if (vk_code == 'S')
				{
					//keyboard_controller->left_thumb.y += is_down ? -1.0f : 1.0f;
					keyboard_controller->left_thumb.y = is_down ? (keyboard_controller->left_thumb.y > 0.0f ? 0.0f : -1.0f)
																											: (keyboard_controller->left_thumb.y < 0.0f ? 0.0f :  1.0f);
					//win32_process_keyboard_message(&keyboard_controller->down, is_down);
				}
				else if (vk_code == 'A')
				{
					//keyboard_controller->left_thumb.x += is_down ? -1.0f : 1.0f;
					keyboard_controller->left_thumb.x = is_down ? (keyboard_controller->left_thumb.x > 0.0f ? 0.0f : -1.0f)
																											: (keyboard_controller->left_thumb.x < 0.0f ? 0.0f : 1.0f);
					//win32_process_keyboard_message(&keyboard_controller->left, is_down);
				}
				else if (vk_code == 'D')
				{
					//keyboard_controller->left_thumb.x += is_down ? 1.0f : -1.0f;
					keyboard_controller->left_thumb.x = is_down ? (keyboard_controller->left_thumb.x < 0.0f ? 0.0f : 1.0f)
																											: (keyboard_controller->left_thumb.x > 0.0f ? 0.0f : -1.0f);
					//win32_process_keyboard_message(&keyboard_controller->right, is_down);
				}
				else if (vk_code == 'Q')
				{
					win32_process_keyboard_message(&keyboard_controller->left_shoulder, is_down);
				}
				else if (vk_code == 'E')
				{
					win32_process_keyboard_message(&keyboard_controller->right_shoulder, is_down);
				}
				else if (vk_code == VK_UP)
				{
					keyboard_controller->right_thumb.y += is_down ? 1.0f : -1.0f;
					//win32_process_keyboard_message(&keyboard_controller->up, is_down);
				}
				else if (vk_code == VK_DOWN)
				{
					keyboard_controller->right_thumb.y += is_down ? -1.0f : 1.0f;
					//win32_process_keyboard_message(&keyboard_controller->down, is_down);
				}
				else if (vk_code == VK_LEFT)
				{
					keyboard_controller->right_thumb.x += is_down ? -1.0f : 1.0f;
					//win32_process_keyboard_message(&keyboard_controller->left, is_down);
				}
				else if (vk_code == VK_RIGHT)
				{
					keyboard_controller->right_thumb.x += is_down ? 1.0f : -1.0f;
					//win32_process_keyboard_message(&keyboard_controller->right, is_down);
				}
				else if (vk_code == VK_ESCAPE)
				{
					global_running = false;
				}
				else if (vk_code == VK_SPACE)
				{
				}
			}
		} break;
		}

		TranslateMessage(&message);
		DispatchMessage(&message);
	}
}

int CALLBACK 
WinMain(HINSTANCE instance, HINSTANCE prev_instance, LPSTR command_line, int show_code)
{
	win32_load_xinput();

	WNDCLASSA window_class = {};
	window_class.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
	window_class.hInstance = instance;
	window_class.lpfnWndProc = MainWindowCallback;
	window_class.lpszClassName = "LocoWindowClass";

	RegisterClass(&window_class);

	HWND window = CreateWindowEx(
		0,
		window_class.lpszClassName,
		"LocoEngine",
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		0,
		0,
		instance,
		0);

	GameInit init_struct = loco_init(__argc, __argv);

	global_locked_mouse = init_struct.locked_mouse;
	if (global_locked_mouse)
	{
		RECT window_rect;
		GetWindowRect(window, &window_rect);
		global_locked_mouse_position.x = window_rect.left + (window_rect.right - window_rect.left) / 2;
		global_locked_mouse_position.y = window_rect.top + (window_rect.bottom - window_rect.top) / 2;
		ShowCursor(false);
	}

	bgfx::winSetHwnd(window);
	loco::init(	init_struct.renderer_type, 
				global_window_width, 
				global_window_height, 
				init_struct.resource_root_path, 
				init_struct.default_resource_relative_path);

	GameInput input[2];
	GameInput* new_input = &input[0];
	GameInput* old_input = &input[1];

	MouseInput zero_mouse = {};
	GameControllerInput zero_controller = {};
	old_input->mouse = zero_mouse;
	for (uint32 i = 0; i < ArrayCount(old_input->controllers); i++)
	{
		old_input->controllers[i] = zero_controller;
		new_input->controllers[i] = zero_controller;
	}

	int64 last_time = loco::hp_counter();
	double clock_freq = double(loco::hp_frequency());

	global_running = true;
	while (global_running)
	{
		int64 now_time = loco::hp_counter();
		float frame_time = (float)(now_time - last_time) / (float)clock_freq;
		last_time = now_time;



		// reset keyboard input
		new_input->keyboard = old_input->keyboard;
		for (int button_index = 0; button_index < ArrayCount(new_input->keyboard.buttons); ++button_index)
			new_input->keyboard.buttons[button_index].transition_count = 0;

		// reset mouse input
		new_input->mouse = old_input->mouse;
		new_input->mouse.abs_move = {};
		for (int button_index = 0; button_index < ArrayCount(new_input->mouse.buttons); ++button_index)
			new_input->mouse.buttons[button_index].transition_count = 0;

		// process windows messages
		win32_process_pending_messages(&new_input->keyboard, &new_input->mouse);

		// get mouse position
		{
			POINT p;
			GetCursorPos(&p);

			new_input->mouse.abs_move.x += p.x - new_input->mouse.abs_pos.x;
			new_input->mouse.abs_move.y += p.y - new_input->mouse.abs_pos.y;
			new_input->mouse.abs_pos.x = p.x;
			new_input->mouse.abs_pos.y = p.y;

			if (global_locked_mouse && global_has_focus)
			{
				new_input->mouse.abs_pos.x = global_locked_mouse_position.x;
				new_input->mouse.abs_pos.y = global_locked_mouse_position.y;
				SetCursorPos(global_locked_mouse_position.x, global_locked_mouse_position.y);
			}
		}

		// get gamepad state
		{
			uint32 max_controller_count = XUSER_MAX_COUNT;
			if (max_controller_count > ArrayCount(new_input->controllers))
				max_controller_count = ArrayCount(new_input->controllers);

			for (DWORD controller_index = 0; controller_index < max_controller_count; ++controller_index)
			{
				GameControllerInput* old_controller = &old_input->gamepad[controller_index];
				GameControllerInput* new_controller = &new_input->gamepad[controller_index];

				XINPUT_STATE controller_state;
				if (XInputGetState(controller_index, &controller_state) == ERROR_SUCCESS)
				{
					// This controller is plugged in
					XINPUT_GAMEPAD* pad = &controller_state.Gamepad;
					win32_process_xinput_digital_buttons(pad->wButtons, XINPUT_GAMEPAD_A, &old_controller->down, &new_controller->down);
					win32_process_xinput_digital_buttons(pad->wButtons, XINPUT_GAMEPAD_B, &old_controller->right, &new_controller->right);
					win32_process_xinput_digital_buttons(pad->wButtons, XINPUT_GAMEPAD_X, &old_controller->left, &new_controller->left);
					win32_process_xinput_digital_buttons(pad->wButtons, XINPUT_GAMEPAD_Y, &old_controller->up, &new_controller->up);
					win32_process_xinput_digital_buttons(pad->wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER, &old_controller->left_shoulder, &new_controller->left_shoulder);
					win32_process_xinput_digital_buttons(pad->wButtons, XINPUT_GAMEPAD_RIGHT_SHOULDER, &old_controller->right_shoulder, &new_controller->right_shoulder);

					win32_process_xinput_analogic_stick(pad->sThumbLX, &new_controller->left_thumb.x);
					win32_process_xinput_analogic_stick(pad->sThumbLY, &new_controller->left_thumb.y);
					win32_process_xinput_analogic_stick(pad->sThumbRX, &new_controller->right_thumb.x);
					win32_process_xinput_analogic_stick(pad->sThumbRY, &new_controller->right_thumb.y);

					new_controller->left_trigger = pad->bLeftTrigger / 255.0f;
					new_controller->right_trigger = pad->bRightTrigger / 255.0f;
				}
				else
				{
					// controller plugged out
				}
			}
		}

		// game update
		loco_update_and_render(frame_time, global_window_width, global_window_height, new_input);

		// swap old/new inputs
		GameInput* temp_input = old_input;
		old_input = new_input;
		new_input = temp_input;
	}

	loco::shutdown();

}

