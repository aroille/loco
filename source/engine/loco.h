
#ifndef LOCO_H_HEADER_GUARD
#define LOCO_H_HEADER_GUARD

#include "core.h"

#include "world.h"
#include "entity.h"
#include "resource_manager.h"

namespace loco
{
	

	extern unsigned resource_root_path_length;
	extern char resource_root_path[LOCO_PATH_LENGTH];
	extern char default_resource_relativ_path[LOCO_PATH_LENGTH];
	
	extern Renderer	renderer;
	extern ResourceManager resources;
	extern Log log;

	void init(const char* resources_path, const char* default_resources_relativ_path);
	void shutdown();

	Entity create_entity();
	bool is_entity_alive(Entity e);
	void destroy_entity(Entity e);
}

#endif // LOCO_H_HEADER_GUARD