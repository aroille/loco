#include "entity_manager.h"

#include "debug.h"

namespace loco
{
	const unsigned ENTITY_MINIMUN_FREE_INDICES = 1024;

	Entity EntityManager::make_entity(unsigned index, char generation)
	{
		return Entity{ 
			(index & ENTITY_INDEX_MASK) | ((generation << ENTITY_INDEX_BITS) & ENTITY_GENERATION_MASK), // id 
		};
	}

	Entity EntityManager::create()
	{
		unsigned idx;
		if (_free_indices.size() > ENTITY_MINIMUN_FREE_INDICES)
		{
			idx = _free_indices.front();
			_free_indices.pop_front();
		}
		else
		{
			_generation.push_back(0);
			idx = _generation.size() - 1;
			check(idx < (1 << ENTITY_INDEX_BITS));
		}
		return make_entity(idx, _generation[idx]);
	}

	bool EntityManager::alive(Entity e) const
	{
		return _generation[e.index()] == e.generation();
	}

	void EntityManager::destroy(Entity e)
	{
		const unsigned idx = e.index();
		++_generation[idx];
		_free_indices.push_back(idx);
	}
}
