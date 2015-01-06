
#ifndef LOCO_H_HEADER_GUARD
#define LOCO_H_HEADER_GUARD

#include "core.h"

#include "world.h"
#include "entity.h"
#include "resource_manager.h"

namespace loco
{
	using namespace resource;

	extern EntityManager entity_manager;
	extern ResourceManager resource_manager;
	extern Renderer	renderer;
	extern Log log;

	extern unsigned resource_root_path_length;
	extern char resource_root_path[LOCO_PATH_LENGTH];
	extern char default_resource_relativ_path[LOCO_PATH_LENGTH];

	void init(const char* resources_path, const char* default_resources_relativ_path);
	void shutdown();
}

#endif // LOCO_H_HEADER_GUARD