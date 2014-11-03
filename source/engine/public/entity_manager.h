#ifndef ENTITY_MANAGER_H_HEADER_GUARD
#define ENTITY_MANAGER_H_HEADER_GUARD

#include "entity.h"

#include <vector>
#include <deque>

namespace loco
{
	class EntityManager
	{
	public:
		Entity	create();
		bool	alive(Entity e) const;
		void	destroy(Entity e);

	private:
		Entity	make_entity(unsigned index, char generation);

		std::vector<char>		_generation;
		std::deque<unsigned>	_free_indices;
	};
}

#endif // ENTITY_MANAGER_H_HEADER_GUARD