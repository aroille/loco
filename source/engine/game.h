#ifndef GAME_H_HEADER_GUARD
#define GAME_H_HEADER_GUARD


#include "world.h"
#include "entity_manager.h"

namespace loco
{
	struct Game
	{
		static void init();
		static void shutdown();

		static EntityManager* entity_manager() { return &_entity_manager; };
		static World* world() { return &_world; };

	private:
		static EntityManager _entity_manager;
		static World _world;
	};
}

#endif // GAME_H_HEADER_GUARD