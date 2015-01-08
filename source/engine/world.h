#ifndef WORLD_H_HEADER_GUARD
#define WORLD_H_HEADER_GUARD

#include "entity.h"
#include "transform_system.h"
#include "mesh_render_system.h"

namespace loco
{
	struct World
	{
		TransformSystem transforms;
		MeshRenderSystem mesh_renders;

		/// Garbage collector
		void gc(const EntityManager& em);

		/// Update all world component systems
		void update();
	};
}

#endif // WORLD_H_HEADER_GUARD