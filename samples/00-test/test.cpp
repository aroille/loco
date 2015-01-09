
#include "loco.h"
#include "entry.h"

#include <bgfxplatform.h>
#include "bgfx_temp.h"
#include <bx/timer.h>

#include <stdint.h>

loco::World main_world;

using loco::Matrix4x4;
using loco::Vector4;
using loco::Vector3;

bgfx::VertexDecl PosColorVertex::ms_decl;


loco::Entity create_axis(loco::World& world, float length, float thickness)
{
	float l = 0.5f * length ;
	float t = 0.5f * thickness;
	float p = l + t;

	loco::Vector3 position[4] = { { 0, 0, 0 }, { p, 0, 0 }, { 0, p, 0 }, { 0, 0, p } };
	loco::Vector3 scale[4] = { { t, t, t }, { l, t, t }, { t, l, t }, { t, t, l } };
	loco::Vector4 color[4] = { { 1, 1, 1, 1 }, { 1, 0, 0, 1 }, { 0, 1, 0, 1 }, { 0, 0, 1, 0 } };

	loco::Entity origin = loco::entity_manager.create();
	loco::TransformSystem::Component origin_tf = world.transform.create(origin);

	for (int i = 0; i < 4; i++)
	{
		// create entity
		loco::Entity e = loco::entity_manager.create();

		// create Transform component
		loco::TransformSystem::Component tf = world.transform.create(e);
		world.transform.link(tf, origin_tf);
		world.transform.set_local_matrix(tf, Matrix4x4{ { { scale[i].x, 0, 0, 0, 0, scale[i].y, 0, 0, 0, 0, scale[i].z, 0, position[i].x, position[i].y, position[i].z, 1 } } });

		// create MeshRender component
		loco::MeshRenderSystem::Component mesh_render = world.mesh_render.create(e);
		loco::Mesh cube = loco::resource_manager.get<loco::Mesh>("loco/mesh/cube").duplicate();
		world.mesh_render.set_mesh(mesh_render, cube);

		// set Material
		loco::Material mat = loco::resource_manager.get<loco::Material>("loco/material/uni_color").duplicate();
		mat->set("u_color", loco::Renderer::UniformType::Vector4, (float*)&color[i]);
		cube->materials[0] = mat;
	}

	return origin;
}

int _main_(int argc, char** argv)
{
	// init loco
	char* resource_root_path = argc > 1 ? argv[1] : "resources/";
	uint32_t width = 1280;
	uint32_t height = 720;

	loco::init(resource_root_path, "loco/");
	loco::entry::set_window_size(loco::entry::WindowHandle{ 0 }, width, height);

	// Create axis display
	loco::Entity axis = create_axis(main_world, 2.0f, 0.2f);
	loco::TransformSystem::Component axis_tf = main_world.transform.lookup(axis);
	main_world.transform.set_local_matrix(axis_tf, Matrix4x4{ { { 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 2.0, 0.0, 1.0 } } });

	// load sponza
	loco::resource_manager.load_folder("sponza/");
	loco::Mesh sponza_mesh = loco::resource_manager.get<loco::Mesh>("sponza/sponza");

	loco::Entity sponza_entity = loco::entity_manager.create();
	loco::TransformSystem::Component sponza_tf = main_world.transform.create(sponza_entity);
	loco::MeshRenderSystem::Component sponza_mr = main_world.mesh_render.create(sponza_entity);
	main_world.mesh_render.set_mesh(sponza_mr, sponza_mesh);

	sponza_mesh->materials[0]  = loco::resource_manager.get<loco::Material>("sponza/material/first_floor_base_arch");
	sponza_mesh->materials[1]  = loco::resource_manager.get<loco::Material>("sponza/material/arch");
	sponza_mesh->materials[2]  = loco::resource_manager.get<loco::Material>("sponza/material/bricks");
	sponza_mesh->materials[3]  = loco::resource_manager.get<loco::Material>("sponza/material/window");
	sponza_mesh->materials[4]  = loco::resource_manager.get<loco::Material>("sponza/material/corner_bricks");
	sponza_mesh->materials[5]  = loco::resource_manager.get<loco::Material>("sponza/material/ceiling");
	sponza_mesh->materials[6]  = loco::resource_manager.get<loco::Material>("sponza/material/door1");
	sponza_mesh->materials[7]  = loco::resource_manager.get<loco::Material>("sponza/material/door2");
	sponza_mesh->materials[8]  = loco::resource_manager.get<loco::Material>("sponza/material/bricks");
	sponza_mesh->materials[9]  = loco::resource_manager.get<loco::Material>("sponza/material/arch");
	sponza_mesh->materials[10] = loco::resource_manager.get<loco::Material>("sponza/material/corner_bricks");
	sponza_mesh->materials[11] = loco::resource_manager.get<loco::Material>("sponza/material/base_column");
	sponza_mesh->materials[12] = loco::resource_manager.get<loco::Material>("sponza/material/base_column");
	sponza_mesh->materials[13] = loco::resource_manager.get<loco::Material>("sponza/material/corner_bricks");
	sponza_mesh->materials[14] = loco::resource_manager.get<loco::Material>("sponza/material/round_column");
	sponza_mesh->materials[15] = loco::resource_manager.get<loco::Material>("sponza/material/base_column");
	sponza_mesh->materials[16] = loco::resource_manager.get<loco::Material>("sponza/material/relief");
	sponza_mesh->materials[17] = loco::resource_manager.get<loco::Material>("sponza/material/base_ceiling");
	sponza_mesh->materials[18] = loco::resource_manager.get<loco::Material>("sponza/material/base_ceiling");
	sponza_mesh->materials[19] = loco::resource_manager.get<loco::Material>("sponza/material/bricks");



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
				
				//mat->set_shader(loco::resource_manager.get<loco::Shader>("loco/shader/vs_default"), loco::resource_manager.get<loco::Shader>("loco/shader/ps_default"));

				float mtx_sponza_scale[16];

				float sponza_scale = 1.0f + fmod(time,1);
				bx::mtxScale(mtx_sponza_scale, sponza_scale, sponza_scale, sponza_scale);
				Matrix4x4 sponza_world_matrix;
				memcpy(sponza_world_matrix.val, mtx_sponza_scale, sizeof(Matrix4x4));
				//main_world.transform.set_local_matrix(tf_1, sponza_world_matrix);


				main_world.update();
				loco::render(main_world);
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
