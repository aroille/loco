#include "world.h"
#include "debug.h"
#include "loco.h"
#include "resource_manager.h"

namespace loco
{
	// Synchronize the updated transform data for each systems of the world
	// (Only meshRender systems are updated by this function for the moment)
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

	// callback, return the world matrix of an entity
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
		camera.gc(em);
		transform.gc(em);
	}

	void World::update()
	{
		batch_transform_sync(*this);
	}


	// Render a world
	void render(const World& world, Entity camera, Viewport viewport)
	{
		uint8 view_id = 0;

		// Get camera component
		CameraSystem::Component camera_cp = world.camera.lookup(camera);
		TransformSystem::Component camera_tf = world.transform.lookup(camera);

		LOCO_ASSERTF(world.camera.is_valid(camera_cp), "", "Rendering issue, camera component is missing");
		LOCO_ASSERTF(world.transform.is_valid(camera_tf), "", "Rendering issue, camera transform is missing");

		// Get view & proj matrix
		float aspect_ratio = (float)viewport.width / (float)viewport.height;

		Matrix4x4 proj_mtx = world.camera.projection_matrix(camera_cp, aspect_ratio);
		Matrix4x4 view_mtx = world.transform.world_matrix(camera_tf);

		// Apply view & proj matrix
		renderer.set_view_rect(view_id, viewport);
		renderer.set_view_transform(view_id, view_mtx, proj_mtx);

		// submit draw call
		const MeshRenderSystem::ComponentData& data = world.mesh_render._data;
		for (int i = 0; i < data.size; i++)
		{
			loco::log.info("WORLD_RENDER", "%s", data.transform[i].to_string().c_str());
		}
		renderer.batch_render(view_id, data.size, data.mesh, data.transform, resource_manager.get_default());
	}

} // loco