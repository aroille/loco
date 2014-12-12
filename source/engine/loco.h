
#ifndef LOCO_H_HEADER_GUARD
#define LOCO_H_HEADER_GUARD

#include "world.h"
#include "entity_manager.h"
#include "resource_manager.h"




namespace loco
{
	extern char resource_root_path[LOCO_PATH_LENGTH];
	extern unsigned resource_root_path_length;

	extern ResourceManager resources;
	extern World world;

	void init(const char* resources_path, const char* loco_resources_relativ_path);
	void shutdown();

	Entity create_entity();
	bool is_entity_alive(Entity e);
	void destroy_entity(Entity e);
}

#endif // LOCO_H_HEADER_GUARD