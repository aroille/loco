#include "world.h"

namespace loco
{
	/// Synchronize the updated transform data of each system of the world
	void batch_transform_sync(World& world)
	{
		TransformSystem::ComponentData data = world.transform._data;
		unsigned updated_count = data.updated_count;

		// update systems
		if (updated_count > 0)
		{
			world.mesh_render.sync_transform(updated_count, data.entity, data.world);

			// reset updated count in transform system
			world.transform._data.updated_count = 0;
		}
	}

	// callback
	Matrix4x4& callback_transform_sync(World* world, Entity entity)
	{
		return world->transform.world_matrix(world->transform.lookup(entity));
	}

	World::World()
		: mesh_render(this, callback_transform_sync)
	{ 
	
	}

	void World::gc(const EntityManager& em)
	{
		transform.gc(em);
	}

	void World::update()
	{
		batch_transform_sync(*this);
	}


} // loco