
#ifndef LOCO_H_HEADER_GUARD
#define LOCO_H_HEADER_GUARD

#include "defines.h"
#include "entity.h"
#include "backend.h"

namespace loco
{
	namespace resource { class ResourceManager; }

	using namespace resource;

	extern EntityManager entity_manager;
	extern ResourceManager resource_manager;
 	extern uint32 current_frame;

	extern size_t resource_root_path_length;
	extern char resource_root_path[LOCO_PATH_LENGTH];
	extern char default_resource_relativ_path[LOCO_PATH_LENGTH];

	bool is_initialized();
	void init(backend::Type renderer_type, int32 window_width, int32 window_height, const char* resources_path, const char* default_resources_relativ_path);
	void shutdown();
	void frame();
}

#endif // LOCO_H_HEADER_GUARD