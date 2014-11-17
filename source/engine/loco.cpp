
#include "loco.h"
#include "renderer.h"
#include "resources.h"

namespace loco
{
	static EntityManager _entity_manager;
	static World _world;

	void init(char* resources_path)
	{
		Renderer::init();
		Renderer::reset(1280, 720);

		loco::resources::init(resources_path);
	}

	void shutdown()
	{
		Renderer::shutdown();
	}

	World* world()
	{
		return &_world;
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