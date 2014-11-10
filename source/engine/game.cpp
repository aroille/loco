#include "game.h"

namespace loco
{
	EntityManager Game::_entity_manager;
	World Game::_world;

	void Game::init()
	{

	}

	void Game::shutdown()
	{

	}

	Entity Game::create_entity()
	{
		return _entity_manager.create();
	}

	bool Game::is_entity_alive(Entity e)
	{
		return _entity_manager.is_alive(e);
	}

	void Game::destroy_entity(Entity e)
	{
		_entity_manager.destroy(e);
	}

}