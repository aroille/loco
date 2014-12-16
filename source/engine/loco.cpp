
#include "loco.h"
#include "renderer.h"

namespace loco
{
	unsigned resource_root_path_length;
	char resource_root_path[LOCO_PATH_LENGTH];
	char default_resource_relativ_path[LOCO_PATH_LENGTH];
	
	ResourceManager resources;
	Log log;

	EntityManager _entity_manager;

	void init(const char* resources_path, const char* default_resources_path)
	{
		log.info(LOCO_LOG, "Initializing");

		// init resources root path
		strcpy(resource_root_path, resources_path);
		resource_root_path_length = strlen(resource_root_path);

		strcpy(default_resource_relativ_path, default_resources_path);

		//init renderer
		renderer::init();
		renderer::reset(1280, 720);

		// load default resources
		loco::resources.load_folder(default_resource_relativ_path);

		// init default resources
		ResourceManager::init_default_resources();
	}

	void shutdown()
	{
		resources.unload_all();
		renderer::shutdown();
	}

	Entity create_entity()
	{
		return _entity_manager.create();
	}

	bool is_entity_alive(Entity e)
	{
		return _entity_manager.is_alive(e);
	}

	void destroy_entity(Entity e)
	{
		_entity_manager.destroy(e);
	}

} // namespace loco