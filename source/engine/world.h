#ifndef WORLD_H_HEADER_GUARD
#define WORLD_H_HEADER_GUARD

#include "entity.h"
#include "transform_system.h"

namespace loco
{
	struct World
	{
		TransformSystem transforms;

		/// Garbage collector
		void gc(const EntityManager& em)
		{
			transforms.gc(em);
		}
	};
}

#endif // WORLD_H_HEADER_GUARD