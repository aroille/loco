
#include "defines.h"
#include "entry.h"
#include "loco.h"
#include "timer.h"
#include "type.h"
#include "bgfxplatform.h"

#include <windows.h>
#include <xinput.h>
#define global_variable static
#define internal static


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


global_variable bool global_running;
global_variable int32 global_window_width;
global_variable int32 global_window_height;

using namespace loco;

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
		case WM_SIZE:
		{
			OutputDebugStringA("WM_SIZE\n");
			global_window_width = LOWORD(l_param);
			global_window_height = HIWORD(l_param);
			loco::renderer.reset(global_window_width, global_window_height);
		} break;

		case WM_DESTROY:
		{
			global_running = false;
		} break;

		case WM_CLOSE:
		{
			global_running = false;
		} break;

		case WM_ACTIVATEAPP:
		{
			OutputDebugStringA("WM_ACTIVEAPP\n");
		} break;

		case WM_SYSKEYDOWN:
		case WM_SYSKEYUP:
		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			uint32 vk_code = w_param;
			bool was_down = ((l_param & (1 << 30)) != 0);
			bool is_down = ((l_param & (1 << 31)) == 0);

			if (is_down != was_down)
			{
				if (vk_code == 'W')
				{
				}
				else if (vk_code == 'S')
				{
				}
				else if (vk_code == 'A')
				{
				}
				else if (vk_code == 'D')
				{
				}
				else if (vk_code == 'Q')
				{
				}
				else if (vk_code == 'E')
				{
				}
				else if (vk_code == VK_UP)
				{
				}
				else if (vk_code == VK_DOWN)
				{
				}
				else if (vk_code == VK_LEFT)
				{
				}
				else if (vk_code == VK_RIGHT)
				{
				}
				else if (vk_code == VK_ESCAPE)
				{
				}
				else if (vk_code == VK_SPACE)
				{
				}
			}
		} break;

		default:
		{
			//OutputDebugStringA("default\n");
			result = DefWindowProc(window, message, w_param, l_param);
		} break;
	}

	return result;
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


	GameInit init_struct;
	game_init(__argc, __argv, &init_struct);

	bgfx::winSetHwnd(window);
	loco::init(	init_struct.renderer_type, 
				global_window_width, 
				global_window_height, 
				init_struct.resource_root_path, 
				init_struct.default_resource_relative_path);

	GameInput input[2];
	GameInput* new_input = &input[0];
	GameInput* old_input = &input[1];

	int64 last_time = loco::hp_counter();
	double clock_freq = double(loco::hp_frequency());

	global_running = true;
	while (global_running)
	{
		int64 now_time = loco::hp_counter();
		float frame_time = (float)(now_time - last_time) / (float)clock_freq;
		last_time = now_time;

		MSG message;
		while (PeekMessage(&message, 0, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
			{
				global_running = false;
			}

			TranslateMessage(&message);
			DispatchMessage(&message);
		}

		// get gamepad state
		uint32 max_controller_count = XUSER_MAX_COUNT;
		if (max_controller_count > ArrayCount(new_input->controllers))
		{
			max_controller_count = ArrayCount(new_input->controllers);
		}

		for (DWORD controller_index = 0; controller_index < max_controller_count; ++controller_index)
		{
			GameControllerInput* old_controller = &old_input->controllers[controller_index];
			GameControllerInput* new_controller = &new_input->controllers[controller_index];

			XINPUT_STATE controller_state;
			if (XInputGetState(controller_index, &controller_state) == ERROR_SUCCESS)
			{
				/// This controller is plugged in
				XINPUT_GAMEPAD* pad = &controller_state.Gamepad;
				win32_process_xinput_digital_buttons(pad->wButtons, XINPUT_GAMEPAD_A, &old_controller->down, &new_controller->down);
				win32_process_xinput_digital_buttons(pad->wButtons, XINPUT_GAMEPAD_B, &old_controller->right, &new_controller->right);
				win32_process_xinput_digital_buttons(pad->wButtons, XINPUT_GAMEPAD_X, &old_controller->left, &new_controller->left);
				win32_process_xinput_digital_buttons(pad->wButtons, XINPUT_GAMEPAD_Y, &old_controller->up, &new_controller->up);
				win32_process_xinput_digital_buttons(pad->wButtons, XINPUT_GAMEPAD_LEFT_SHOULDER, &old_controller->left_shoulder, &new_controller->left_shoulder);
				win32_process_xinput_digital_buttons(pad->wButtons, XINPUT_GAMEPAD_RIGHT_SHOULDER, &old_controller->right_shoulder, &new_controller->right_shoulder);

				win32_process_xinput_analogic_stick(pad->sThumbLX, &new_controller->left_thumb_x);
				win32_process_xinput_analogic_stick(pad->sThumbLY, &new_controller->left_thumb_y);
				win32_process_xinput_analogic_stick(pad->sThumbRX, &new_controller->right_thumb_x);
				win32_process_xinput_analogic_stick(pad->sThumbRY, &new_controller->right_thumb_y);

				new_controller->left_trigger = pad->bLeftTrigger / 255.0f;
				new_controller->right_trigger = pad->bRightTrigger / 255.0f;
			}
			else
			{
				// controller plugged out
			}
		}

		game_update_and_render(frame_time, global_window_width, global_window_height, new_input);

		// swap old/new inputs
		GameInput* temp_input = new_input;
		old_input = new_input;
		new_input = temp_input;
	}

	loco::shutdown();

}

