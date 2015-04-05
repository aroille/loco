#include "world.h"
#include "debug.h"
#include "loco.h"
#include "resource_manager.h"

namespace loco
{
	// Synchronize the updated transform data for each systems of the world
	// (Only meshRender system is updated by this function for the moment)
	void batch_transform_sync(World& world)
	{
		TransformSystem::ComponentData data = world.transform._data;
		unsigned dirty_count = data.dirty;

		// update systems
		if (dirty_count > 0)
		{
			world.mesh_render.sync_transform(dirty_count, data.entity, data.world);

			// reset updated count in transform system
			world.transform._data.dirty = 0;
		}
	}

	// callback, return the world matrix of an entity
	Matrix4x4& transform_sync_callback(World* world, Entity entity)
	{
		return world->transform.world_matrix(world->transform.lookup(entity));
	}

	World::World()
		: mesh_render(this, transform_sync_callback)
	{ 
	
	}

	void World::gc(const EntityManager& em)
	{
		camera.gc(em);
		transform.gc(em);
	}

	void World::update()
	{
		batch_transform_sync(*this);
	}

} // loco