
#include "loco.h"
#include "renderer.h"



namespace loco
{
	ResourceManager resources;
	World world;

	EntityManager _entity_manager;

	void init(const char* resources_path)
	{
		renderer::init();
		renderer::reset(1280, 720);

		loco::resources.init(resources_path);
		loco::resources.load_folder(LOCO_COMMON_RESOURCE_PATH);
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