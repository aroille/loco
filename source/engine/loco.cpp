
#include "loco.h"
#include "renderer.h"



namespace loco
{
	char resource_root_path[LOCO_PATH_LENGTH];
	unsigned resource_root_path_length;

	ResourceManager resources;

	EntityManager _entity_manager;

	void init(const char* resources_path, const char* loco_resources_relativ_path)
	{
		// init resources root path
		strcpy(resource_root_path, resources_path);
		resource_root_path_length = strlen(resource_root_path);

		//init renderer
		renderer::init();
		renderer::reset(1280, 720);

		// load all resources inside resources_root_path
		loco::resources.load_folder(loco_resources_relativ_path);
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