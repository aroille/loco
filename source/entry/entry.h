#ifndef ENTRY_H_HEADER_GUARD
#define ENTRY_H_HEADER_GUARD

#include "defines.h"
#include "type.h"
#include "renderer.h"


namespace loco
{
	struct GameInit
	{
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
		float x;
		float y;

		float left_thumb_x;
		float left_thumb_y;
		float right_thumb_x;
		float right_thumb_y;

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

	struct GameInput
	{
		GameControllerInput controllers[5];
	};

}

extern void	game_init(int argc, char** argv, loco::GameInit* game_init);
extern void game_update_and_render(float delta_time, int32 window_width, int32 window_height, loco::GameInput* input);


#endif // ENTRY_H_HEADER_GUARD
