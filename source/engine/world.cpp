#include "world.h"

namespace loco
{
	void batch_transform_sync(World& world)
	{
		TransformSystem::ComponentData data = world.transforms._data;
		unsigned updated_count = data.updated_count;

		// update MeshRender components
		world.mesh_renders.sync_transform(updated_count, data.entity, data.world);

		world.transforms._data.updated_count = 0;
	}

	void World::gc(const EntityManager& em)
	{
		transforms.gc(em);
	}

	void World::update()
	{
		batch_transform_sync(*this);
	}

} // loco