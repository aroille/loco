
#include "loco.h"
#include "math_types.h"
#include "bgfx.h"
#include "defines.h"
#include "entry.h"
#include "resource_manager.h"

#include <bgfxplatform.h>

using loco::Matrix4x4;

typedef loco::ResourceManager::Texture Texture;


int _main_(int argc, char** argv)
{
	char* root_resource_path = argc > 1 ? argv[1] : LOCO_DEFAULT_RESOURCE_PATH;
	loco::init(root_resource_path);
	
	loco::entry::set_window_size(loco::entry::WindowHandle{ 0 }, 1280, 720);

	loco::TransformSystem* transform_components = loco::world.transform_system();

	// Create entities
	loco::Entity e_1 = loco::create_entity();
	loco::Entity e_2 = loco::create_entity();
	loco::Entity e_3 = loco::create_entity();
	loco::Entity e_4 = loco::create_entity();
	loco::Entity e_5 = loco::create_entity();

	// Add transform component to each entities
	loco::TransformComponent tf_1 = transform_components->create(e_1);
	loco::TransformComponent tf_2 = transform_components->create(e_2);
	loco::TransformComponent tf_3 = transform_components->create(e_3);
	loco::TransformComponent tf_4 = transform_components->create(e_4);
	loco::TransformComponent tf_5 = transform_components->create(e_5);

	
	Texture tex = loco::resources.get<Texture>("common/mesh/box");
	bool b = loco::resources.is_loaded(tex);
	
	/*
	loco::MeshComponent mesh_cp = mesh_components->create(e_1);
	mesh_components->set_mesh(mesh_cp, mesh);
	*/
	
	/*
	loco::resources::load_folder("common");
	loco::resources::load_folder("level\01");

	//LOCO_RESOURCE
	loco::resources::Mesh mesh = loco::resources::get<loco::resources::Mesh>("common\mesh\box");
	if (mesh == loco::resources::invalid)
	{
		mesh_components->set_mesh(mesh_cp, mesh);
	}
	loco::resources::Mesh mesh = loco::resources::get<loco::resources::Mesh>("common\mesh\box");

	loco::resources::Mesh mesh = loco::resources::get(loco::resources::Type::Enum::MESH, "common\mesh\box");
	loco::resources::Mesh mesh = loco::resources::get_mesh("common\mesh\box");

	loco::resources::Texture texture = loco::resources::get_texture("common\texture\box");
	
	loco::MeshComponent mesh_cp = mesh_components->create(e_1);
	mesh_components->set_mesh(mesh_cp, mesh);
	mesh_components->set_material(mesh_cp, mat);


	loco::resources::unload("level\01");
	loco::resources::unload("common");
	*/
		
	// Create parent/child relations between the transform 
	
	transform_components->link(tf_2, tf_1);
	transform_components->link(tf_3, tf_2);
	transform_components->link(tf_4, tf_2);
	transform_components->link(tf_5, tf_2);
	
	// setting the local transform matrix for each transform component
	transform_components->set_local_matrix(tf_1, Matrix4x4{{{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1 }}});
	transform_components->set_local_matrix(tf_2, Matrix4x4{{{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 2, 0, 0, 1 }}});
	transform_components->set_local_matrix(tf_3, Matrix4x4{{{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 3, 0, 0, 1 }}});
	transform_components->set_local_matrix(tf_4, Matrix4x4{{{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 4, 0, 0, 1 }}});
	transform_components->set_local_matrix(tf_5, Matrix4x4{{{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 5, 0, 0, 1 }}});

	// destroy a parent/child relation
	transform_components->unlink(tf_3);

	// get the world transform matrix 
	Matrix4x4 tf_world_1 = transform_components->world_matrix(tf_1);
	Matrix4x4 tf_world_2 = transform_components->world_matrix(tf_2);
	Matrix4x4 tf_world_3 = transform_components->world_matrix(tf_3);
	Matrix4x4 tf_world_4 = transform_components->world_matrix(tf_4);
	Matrix4x4 tf_world_5 = transform_components->world_matrix(tf_5);
	
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
		//bgfx::reset(720, 720, reset);
		// Set view 0 default viewport.
		bgfx::setViewRect(0, 0, 0, 1280, 720);

		// This dummy draw call is here to make sure that view 0 is cleared
		// if no other draw calls are submitted to view 0.
		bgfx::submit(0);
		bgfx::frame();
	}

	loco::shutdown();

	return 0;
}
