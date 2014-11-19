
#include "loco.h"
#include "renderer.h"

namespace loco
{
	static EntityManager _entity_manager;

	void init(const char* resources_path)
	{
		renderer::init();
		renderer::reset(1280, 720);

		loco::resources.init(resources_path);
	}

	void shutdown()
	{
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