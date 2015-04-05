#include "renderers.h"
#include "backend.h"
#include "resource_manager.h"
#include "world.h"
#include "bgfx.h"

namespace loco
{
	#define LOCO_TO_BGFX(handle) { handle.idx }

	void DeferredResources::update(uint32 width, uint32 height)
	{
		bool need_update = (last_width != width) || (last_height != height);
		if (need_update)
		{
			// TODO
		}
	}

	//==========================================================================
	static void bind_material(const MaterialData* material, const DefaultResources& default_resources)
	{
		const MaterialData* m = (material == nullptr) ? default_resources.material.get() : material;

		// programs
		bgfx::setProgram(LOCO_TO_BGFX(m->_program));

		// numeric parameters
		{
			std::vector<MaterialData::UniformInfo>::const_iterator it = m->_uniform_infos.cbegin();
			while (it != m->_uniform_infos.cend())
			{
				bgfx::setUniform(LOCO_TO_BGFX((*it).uniform), m->_uniform_buffer.data() + (*it).buffer_offset, (*it).array_size);
				it++;
			}
		}

		// samplers
		{
			uint8 tex_unit = 0;
			std::vector<MaterialData::TextureInfo>::const_iterator it = m->_texture_infos.cbegin();
			while (it != m->_texture_infos.cend())
			{
				const Texture& texture = ((*it).texture == Texture::invalid) ? default_resources.texture : (*it).texture;
				bgfx::setTexture(tex_unit, LOCO_TO_BGFX((*it).uniform), LOCO_TO_BGFX(texture), (*it).flags);
				tex_unit++;
				it++;
			}
		}

		// render states
		bgfx::setState(0 \
			| BGFX_STATE_RGB_WRITE \
			| BGFX_STATE_ALPHA_WRITE \
			| BGFX_STATE_DEPTH_TEST_LESS \
			| BGFX_STATE_DEPTH_WRITE \
			| BGFX_STATE_CULL_CCW \
			| BGFX_STATE_MSAA \
			);
	}

	//==========================================================================
	static void batch_render(uint8 view_id, uint32 count, const Mesh* meshes, const Matrix4x4* transforms, const DefaultResources& default_resources)
	{
		for (unsigned i = 0; i < count; i++)
		{
			const Mesh& m = (*meshes == Mesh::invalid) ? default_resources.mesh : *meshes;
			const void* world_matrix = (void*)transforms;

			for (unsigned j = 0; j < m->submeshes.size(); j++)
			{
				// set material
				bind_material(m->materials[j].get(), default_resources);

				// set model matrix
				bgfx::setTransform(world_matrix);

				// set vertex and index buffer
				bgfx::setVertexBuffer(bgfx::VertexBufferHandle LOCO_TO_BGFX(m->submeshes[j].vertex_buffer));
				bgfx::setIndexBuffer(bgfx::IndexBufferHandle LOCO_TO_BGFX(m->submeshes[j].index_buffer));

				// submit draw call
				bgfx::submit(view_id);
			}

			meshes++;
			transforms++;
		}
	}

	//==========================================================================
	void renderer_deferred(const World& world, Entity camera, Viewport viewport, DeferredResources& deferred_resources)
	{
		uint8 view_id = 0;

		// Set view 0 clear state.
		bgfx::setViewClear(	view_id,
												BGFX_CLEAR_COLOR_BIT | BGFX_CLEAR_DEPTH_BIT,
												0x101010ff,
												1.0f,
												0);

		// Get camera component
		CameraComponent camera_cp = world.camera.lookup(camera);
		TransformComponent camera_tf = world.transform.lookup(camera);

		LOCO_ASSERTF(world.camera.is_valid(camera_cp), "", "Rendering issue, camera component is missing");
		LOCO_ASSERTF(world.transform.is_valid(camera_tf), "", "Rendering issue, camera transform is missing");

		// Get view & proj matrix
		float aspect_ratio = (float)viewport.width / (float)viewport.height;

		Matrix4x4 proj_mtx = world.camera.projection_matrix(camera_cp, aspect_ratio);
		Matrix4x4 view_mtx = world.transform.world_matrix(camera_tf);

		// Apply view & proj matrix
		backend::set_view_rect(view_id, viewport);
		backend::set_view_transform(view_id, view_mtx, proj_mtx);

		// submit draw call
		const MeshRenderSystem::ComponentData& data = world.mesh_render._data;
		batch_render(view_id, data.size, data.mesh, data.transform, resource_manager.get_default());
	}
}