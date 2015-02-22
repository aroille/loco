#ifndef ENTRY_H_HEADER_GUARD
#define ENTRY_H_HEADER_GUARD

#include "defines.h"
#include "type.h"
#include "renderer.h"


namespace loco
{
	struct GameInit
	{
		bool locked_mouse;
		Renderer::Type::Enum renderer_type;
		char resource_root_path[LOCO_PATH_LENGTH];
		char default_resource_relative_path[LOCO_PATH_LENGTH];
	};

	struct GameButtonState
	{
		int32 transition_count;
		bool is_down;
	};

	struct GameControllerInput
	{
		Vector2 left_thumb;
		Vector2 right_thumb;
		float left_trigger;
		float right_trigger;

		union
		{
			GameButtonState buttons[6];
			struct
			{
				GameButtonState up;
				GameButtonState down;
				GameButtonState left;
				GameButtonState right;
				GameButtonState left_shoulder;
				GameButtonState right_shoulder;
			};
		};
	};

	struct MouseInput
	{
		Vector2i abs_pos;
		Vector2i abs_move;
		float wheel;

		union
		{
			GameButtonState buttons[3];
			struct
			{
				GameButtonState left;
				GameButtonState middle;
				GameButtonState right;
			};
		};
	};

	struct GameInput
	{
		MouseInput							mouse;
		union
		{
			GameControllerInput		controllers[5];
			struct
			{
				GameControllerInput keyboard;
				GameControllerInput gamepad[4];
			};
		};
	};

}

extern void	loco_init(int argc, char** argv, loco::GameInit* game_init);
extern void loco_update_and_render(float delta_time, int32 window_width, int32 window_height, loco::GameInput* input);


#endif // ENTRY_H_HEADER_GUARD
