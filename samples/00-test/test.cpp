
#include "loco.h"
#include "math_types.h"
#include "defines.h"
#include "entry.h"
#include "resource_manager.h"
#include "resource_material.h"
#include <bgfxplatform.h>
#include "bgfx_temp.h"

using loco::Matrix4x4;

bgfx::VertexDecl PosColorVertex::ms_decl;

int _main_(int argc, char** argv)
{
	uint32_t width = 1280;
	uint32_t height = 720;
	
	char* resource_root_path = argc > 1 ? argv[1] : "resources/";

	// init loco
	loco::init(resource_root_path);	
	loco::entry::set_window_size(loco::entry::WindowHandle{ 0 }, width, height);
	
	// Create entities
	loco::Entity e_1 = loco::create_entity();
	loco::Entity e_2 = loco::create_entity();
	loco::Entity e_3 = loco::create_entity();
	loco::Entity e_4 = loco::create_entity();
	loco::Entity e_5 = loco::create_entity();

	// Add transform component to each entities
	loco::TransformComponent tf_1 = loco::world.transforms.create(e_1);
	loco::TransformComponent tf_2 = loco::world.transforms.create(e_2);
	loco::TransformComponent tf_3 = loco::world.transforms.create(e_3);
	loco::TransformComponent tf_4 = loco::world.transforms.create(e_4);
	loco::TransformComponent tf_5 = loco::world.transforms.create(e_5);

	// test load mesh
	loco::Mesh mesh = loco::resources.get<loco::Mesh>("common/mesh/bunny");

	// test load texture
	loco::ResourceName res_name = loco::resources.get_name("common/texture/biodome_floor_04a");
	loco::Texture texture1 = loco::resources.get<loco::Texture>("common/texture/biodome_floor_04a");
	loco::Texture texture2 = loco::resources.get<loco::Texture>(res_name);

	// test load shaders
	loco::Shader vertex_shader = loco::resources.get<loco::Shader>("shaders/dx9/vs_cubes");
	loco::Shader pixel_shader = loco::resources.get<loco::Shader>("shaders/dx9/fs_cubes");

	// test create material
	float time = 5.0f;
	float color[3] = { 0.1f, 0.5f, 0.8f };
	
	loco::Material material;
	material.set_shader(vertex_shader, pixel_shader);

	//material->set("u_time", loco::renderer::UniformType::Float, &time);
	//material->set("u_color", loco::renderer::UniformType::Vector3, &color[0]);
	//material->set("u_diffuse", texture1, LOCO_TEXTURE_U_CLAMP | LOCO_TEXTURE_V_CLAMP);
	
	//loco::MeshRendererComponent mesh_renderer_cpn = mesh_render_components->create(e_1);

	//bool b = loco::resources.is_loaded(texture);

	/*
	loco::MeshComponent mesh_cp = mesh_components->create(e_1);
	mesh_components->set_mesh(mesh_cp, mesh);
	*/
	
	/*

	//LOCO_RESOURCE
	loco::resources::Mesh mesh = loco::resources::get<loco::resources::Mesh>("common\mesh\box");
	if (mesh == loco::resources::invalid)
	{
		mesh_components->set_mesh(mesh_cp, mesh);
	}
	loco::resources::Mesh mesh = loco::resources::get<loco::resources::Mesh>("common\mesh\box");

	loco::resources::Mesh mesh = loco::resources::get(loco::resources::Type::Enum::MESH, "common\mesh\box");
	loco::resources::Mesh mesh = loco::resources::get_mesh("common\mesh\box");
	
	loco::MeshComponent mesh_cp = mesh_components->create(e_1);
	mesh_components->set_mesh(mesh_cp, mesh);
	mesh_components->set_material(mesh_cp, mat);

	loco::resources::unload("level\01");
	loco::resources::unload("common");
	*/
		
	// Create parent/child relations between the transform 
	loco::world.transforms.link(tf_2, tf_1);
	loco::world.transforms.link(tf_3, tf_2);
	loco::world.transforms.link(tf_4, tf_2);
	loco::world.transforms.link(tf_5, tf_2);
	
	// setting the local transform matrix for each transform component
	loco::world.transforms.set_local_matrix(tf_1, Matrix4x4{ { { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1 } } });
	loco::world.transforms.set_local_matrix(tf_2, Matrix4x4{ { { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 2, 0, 0, 1 } } });
	loco::world.transforms.set_local_matrix(tf_3, Matrix4x4{ { { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 3, 0, 0, 1 } } });
	loco::world.transforms.set_local_matrix(tf_4, Matrix4x4{ { { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 4, 0, 0, 1 } } });
	loco::world.transforms.set_local_matrix(tf_5, Matrix4x4{ { { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 5, 0, 0, 1 } } });

	// destroy a parent/child relation
	loco::world.transforms.unlink(tf_3);

	// get the world transform matrix 
	Matrix4x4 tf_world_1 = loco::world.transforms.world_matrix(tf_1);
	Matrix4x4 tf_world_2 = loco::world.transforms.world_matrix(tf_2);
	Matrix4x4 tf_world_3 = loco::world.transforms.world_matrix(tf_3);
	Matrix4x4 tf_world_4 = loco::world.transforms.world_matrix(tf_4);
	Matrix4x4 tf_world_5 = loco::world.transforms.world_matrix(tf_5);
	
	// Create vertex stream declaration.
	PosColorVertex::init();

	// Create static vertex buffer.
	bgfx::VertexBufferHandle vbh = bgfx::createVertexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices))
		, PosColorVertex::ms_decl
		);

	// Create static index buffer.
	bgfx::IndexBufferHandle ibh = bgfx::createIndexBuffer(
		// Static data can be passed with bgfx::makeRef
		bgfx::makeRef(s_cubeIndices, sizeof(s_cubeIndices))
		);

	// Create program from shaders.
	bgfx::ProgramHandle program = loadProgram("vs_cubes", "fs_cubes");

	float at[3] = { 0.0f, 0.0f, 0.0f };
	float eye[3] = { 0.0f, 0.0f, -35.0f };

	// Set view 0 clear state.
	bgfx::setViewClear(0
		, BGFX_CLEAR_COLOR_BIT | BGFX_CLEAR_DEPTH_BIT
		, 0x303030ff
		, 1.0f
		, 0
		);

	uint32_t debug = BGFX_DEBUG_TEXT;
	uint32_t reset = BGFX_RESET_VSYNC;

	loco::entry::WindowState state;
	while (!loco::entry::process_window_events(state, debug, reset))
	//while (true)
	{
		float view[16];
		float proj[16];
		bx::mtxLookAt(view, eye, at);
		bx::mtxProj(proj, 60.0f, float(width) / float(height), 0.1f, 100.0f);

		// Set view and projection matrix for view 0.
		bgfx::setViewTransform(0, view, proj);

		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, width, height);

		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, 1280, 720);



		// This dummy draw call is here to make sure that view 0 is cleared
		// if no other draw calls are submitted to view 0.
		bgfx::submit(0);

		// Submit 11x11 cubes.
		for (uint32_t yy = 0; yy < 11; ++yy)
		{
			for (uint32_t xx = 0; xx < 11; ++xx)
			{
				float mtx[16];
				bx::mtxRotateXY(mtx, 0 + xx*0.21f, 0 + yy*0.37f);
				mtx[12] = -15.0f + float(xx)*3.0f;
				mtx[13] = -15.0f + float(yy)*3.0f;
				mtx[14] = 0.0f;

				loco::renderer::submit(0, mesh, material, mtx);
			}
		}

		bgfx::frame();
	}

	loco::shutdown();

	return 0;
}
