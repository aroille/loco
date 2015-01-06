
#include "loco.h"
#include "entry.h"

#include <bgfxplatform.h>
#include "bgfx_temp.h"
#include <bx/timer.h>

#include <stdint.h>

loco::World main_world;

using loco::Matrix4x4;
bgfx::VertexDecl PosColorVertex::ms_decl;

int _main_(int argc, char** argv)
{


	// init loco
	char* resource_root_path = argc > 1 ? argv[1] : "resources/";
	uint32_t width = 1280;
	uint32_t height = 720;

	loco::init(resource_root_path, "loco/");
	loco::entry::set_window_size(loco::entry::WindowHandle{ 0 }, width, height);

	// load sponza
	loco::resource_manager.load_folder("sponza/");

	// Create entities
	loco::Entity e_1 = loco::entity_manager.create();
	loco::Entity e_2 = loco::entity_manager.create();
	loco::Entity e_3 = loco::entity_manager.create();
	loco::Entity e_4 = loco::entity_manager.create();
	loco::Entity e_5 = loco::entity_manager.create();

	// Add transform component to each entities
	loco::TransformSystem::Component tf_1 = main_world.transforms.create(e_1);
	loco::TransformSystem::Component tf_2 = main_world.transforms.create(e_2);
	loco::TransformSystem::Component tf_3 = main_world.transforms.create(e_3);
	loco::TransformSystem::Component tf_4 = main_world.transforms.create(e_4);
	loco::TransformSystem::Component tf_5 = main_world.transforms.create(e_5);

	// test load mesh
	loco::Mesh mesh = loco::resource_manager.get<loco::Mesh>("sponza/sponza");
	//loco::Mesh mesh = loco::resources.get<loco::Mesh>("loco/mesh/bunny");

	// test load texture
	loco::ResourceName res_name = loco::resource_manager.get_name("loco/texture/default");
	loco::Texture texture1 = loco::resource_manager.get<loco::Texture>("loco/texture/default");
	loco::Texture texture2 = loco::resource_manager.get<loco::Texture>(res_name);

	// test load shaders
	loco::Shader vertex_shader = loco::resource_manager.get<loco::Shader>("loco/shader/vs_default");
	loco::Shader pixel_shader = loco::resource_manager.get<loco::Shader>("loco/shader/ps_default");

	// test load material
	loco::Material simple_mat = loco::resource_manager.get<loco::Material>("sponza/sponza");

	/*
	loco::MeshComponent mesh_cp = main_world.mesh_components->create(e_1);
	mesh_cp->set_mesh(mesh_cp, mesh);
	mesh_cp->set_material(mesh_cp, mat);
	*/
		
	// Create parent/child relations between the transform 
	main_world.transforms.link(tf_2, tf_1);
	main_world.transforms.link(tf_3, tf_2);
	main_world.transforms.link(tf_4, tf_2);
	main_world.transforms.link(tf_5, tf_2);
	
	// setting the local transform matrix for each transform component
	main_world.transforms.set_local_matrix(tf_1, Matrix4x4{ { { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1 } } });
	main_world.transforms.set_local_matrix(tf_2, Matrix4x4{ { { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 2, 0, 0, 1 } } });
	main_world.transforms.set_local_matrix(tf_3, Matrix4x4{ { { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 3, 0, 0, 1 } } });
	main_world.transforms.set_local_matrix(tf_4, Matrix4x4{ { { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 4, 0, 0, 1 } } });
	main_world.transforms.set_local_matrix(tf_5, Matrix4x4{ { { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 5, 0, 0, 1 } } });

	// destroy a parent/child relation
	main_world.transforms.destroy(e_2);
	main_world.transforms.unlink(tf_3);

	// get the world transform matrix 
	Matrix4x4 tf_world_1 = main_world.transforms.world_matrix(tf_1);
	//Matrix4x4 tf_world_2 = main_world.transforms.world_matrix(tf_2);
	Matrix4x4 tf_world_3 = main_world.transforms.world_matrix(tf_3);
	Matrix4x4 tf_world_4 = main_world.transforms.world_matrix(tf_4);
	Matrix4x4 tf_world_5 = main_world.transforms.world_matrix(tf_5);

	float at[3] = { 0.0f, 0.0f, 0.0f };
	float eye[3] = { 0.0f, 0.0f, -35.0f };

	// Set view 0 clear state.
	bgfx::setViewClear(0
		, BGFX_CLEAR_COLOR_BIT | BGFX_CLEAR_DEPTH_BIT
		, 0x101010ff
		, 1.0f
		, 0
		);


	loco::cmdAdd("move", cmdMove);
	loco::inputAddBindings("camBindings", s_camBindings);
	cameraReset();

	uint32_t debug = BGFX_DEBUG_TEXT;
	uint32_t reset = BGFX_RESET_VSYNC;

	int64_t timeOffset = bx::getHPCounter();

	//loco::entry::WindowState state;
	//while (!loco::entry::process_window_events(state, debug, reset))

	loco::entry::MouseState mouseState;
	while (!loco::entry::process_events(width, height, debug, reset, &mouseState))
	{
		int64_t now = bx::getHPCounter();
		static int64_t last = now;
		const int64_t frameTime = now - last;
		last = now;
		const double freq = double(bx::getHPFrequency());
		const double toMs = 1000.0 / freq;
		const float deltaTime = float(frameTime / freq);

		float time = (float)((now - timeOffset) / freq);

		loco::resource_manager.hot_reload<loco::Shader>();
		loco::resource_manager.hot_reload<loco::Material>();
		loco::resource_manager.hot_reload<loco::Mesh>();

		float view[16];
		float proj[16];
		cameraUpdate(deltaTime, mouseState.m_mx, mouseState.m_my, !!mouseState.m_buttons[loco::entry::MouseButton::Right]);
		bx::mtxLookAt(view, m_eye, m_at, m_up);
		bx::mtxProj(proj, 60.0f, float(width) / float(height), 0.1f, 1000.0f);

		// Set view and projection matrix for view 0.
		bgfx::setViewTransform(0, view, proj);

		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, width, height);

		// This dummy draw call is here to make sure that view 0 is cleared
		// if no other draw calls are submitted to view 0.
		bgfx::submit(0);

		// Submit 11x11 cubes.
		for (uint32_t yy = 0; yy < 1; ++yy)
		{
			for (uint32_t xx = 0; xx < 1; ++xx)
			{
				float mtx_pos[16];
				bx::mtxRotateXY(mtx_pos, 0 + xx*0.21f, 0 + yy*0.37f);
				mtx_pos[12] = -15.0f + float(xx)*3.0f;
				mtx_pos[13] = -15.0f + float(yy)*3.0f;
				mtx_pos[14] = 0.0f;

				float mtx_scale[16];
				float scale = 10.0f;
				bx::mtxScale(mtx_scale, scale, scale, scale);

				float mtx[16];
				bx::mtxMul(mtx, mtx_scale, mtx_pos);
				
				simple_mat->set_shader(loco::resource_manager.get<loco::Shader>("loco/shader/vs_default"), loco::resource_manager.get<loco::Shader>("loco/shader/ps_default"));
				loco::renderer.submit(0, mesh, mtx, loco::resource_manager.get_default());
			}
		}

		//loco::renderer::render(0, world, camera, viewport);
		bgfx::frame();

		/// garbage collector
		main_world.gc(loco::entity_manager);
	}

	loco::shutdown();

	return 0;
}
