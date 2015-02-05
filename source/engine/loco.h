
#ifndef LOCO_H_HEADER_GUARD
#define LOCO_H_HEADER_GUARD

#include "defines.h"
#include "entity.h"
#include "log.h"
#include "renderer.h"

namespace loco
{
	namespace resource { class ResourceManager; }
	class Renderer;

	using namespace resource;

	extern EntityManager entity_manager;
	extern ResourceManager resource_manager;
	extern Renderer	renderer;
	extern Log log;
	extern uint32 current_frame;

	extern size_t resource_root_path_length;
	extern char resource_root_path[LOCO_PATH_LENGTH];
	extern char default_resource_relativ_path[LOCO_PATH_LENGTH];

	void init(Renderer::Type::Enum renderer_type, int32 window_width, int32 window_height, const char* resources_path, const char* default_resources_relativ_path);
	void shutdown();
	void frame();
}

#endif // LOCO_H_HEADER_GUARD