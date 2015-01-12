#ifndef WORLD_H_HEADER_GUARD
#define WORLD_H_HEADER_GUARD

#include "entity.h"
#include "camera_system.h"
#include "mesh_render_system.h"
#include "transform_system.h"

namespace loco
{
	struct World
	{
		CameraSystem camera;
		MeshRenderSystem mesh_render;
		TransformSystem transform;

		World();

		/// Garbage collector
		void gc(const EntityManager& em);

		/// Update all world component systems
		void update();
	};

	extern void render(const World& world, Entity camera, Viewport viewport);
}

#endif // WORLD_H_HEADER_GUARD