
#include "loco.h"
#include "resource_manager.h"
#include "world.h"
#include "type.h"
#include "entry.h"
#include "renderers.h"
#include "../common/sample_common.h"

using namespace loco;
using namespace loco::math;

static World world;
static Entity camera;
static DeferredResources defered_resources;

Entity create_sponza(World& world)
{
	Entity sponza_entity = entity_manager.create();
	TransformComponent sponza_tf = world.transform.create(sponza_entity);
	MeshRenderComponent sponza_mr = world.mesh_render.create(sponza_entity);

	Mesh sponza_mesh = resource_manager.get<Mesh>("sponza/sponza");
	world.mesh_render.set_mesh(sponza_mr, sponza_mesh);

	sponza_mesh->materials[0] = resource_manager.get<Material>("sponza/material/first_floor_base_arch");
	sponza_mesh->materials[1] = resource_manager.get<Material>("sponza/material/arch");
	sponza_mesh->materials[2] = resource_manager.get<Material>("sponza/material/bricks");
	sponza_mesh->materials[3] = resource_manager.get<Material>("sponza/material/window");
	sponza_mesh->materials[4] = resource_manager.get<Material>("sponza/material/corner_bricks");
	sponza_mesh->materials[5] = resource_manager.get<Material>("sponza/material/ceiling");
	sponza_mesh->materials[6] = resource_manager.get<Material>("sponza/material/door1");
	sponza_mesh->materials[7] = resource_manager.get<Material>("sponza/material/door2");
	sponza_mesh->materials[8] = resource_manager.get<Material>("sponza/material/bricks");
	sponza_mesh->materials[9] = resource_manager.get<Material>("sponza/material/arch");
	sponza_mesh->materials[10] = resource_manager.get<Material>("sponza/material/corner_bricks");
	sponza_mesh->materials[11] = resource_manager.get<Material>("sponza/material/base_column");
	sponza_mesh->materials[12] = resource_manager.get<Material>("sponza/material/base_column");
	sponza_mesh->materials[13] = resource_manager.get<Material>("sponza/material/corner_bricks");
	sponza_mesh->materials[14] = resource_manager.get<Material>("sponza/material/round_column");
	sponza_mesh->materials[15] = resource_manager.get<Material>("sponza/material/base_column");
	sponza_mesh->materials[16] = resource_manager.get<Material>("sponza/material/relief");
	sponza_mesh->materials[17] = resource_manager.get<Material>("sponza/material/base_ceiling");
	sponza_mesh->materials[18] = resource_manager.get<Material>("sponza/material/base_ceiling");
	sponza_mesh->materials[19] = resource_manager.get<Material>("sponza/material/bricks");

	return sponza_entity;
}

void init_scene()
{
	// Create axis display
	Entity axis = create_axis(world, 2.0f, 0.2f);
	TransformComponent axis_tf = world.transform.lookup(axis);
	world.transform.set_local_matrix(axis_tf, Matrix4x4(0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 2.0, 0.0, 1.0));

	// Create sponza
	resource_manager.load_folder("sponza/");
	Entity sponza_entity = create_sponza(world);

	// Create camera
	camera = entity_manager.create();
	CameraComponent camera_cp = world.camera.create(camera);
	TransformComponent camera_tf = world.transform.create(camera);
}

GameInit loco_init(int argc, char** argv)
{
	GameInit game_init = {};

	// lock and hide mouse
	game_init.locked_mouse = true;

	// let the engine choose the graphic backend
	game_init.renderer_type = backend::Type::Count;

	// set the resource paths
	strcpy_s(game_init.resource_root_path, sizeof(game_init.resource_root_path), argc > 1 ? argv[1] : "resources/");
	strcpy_s(game_init.default_resource_relative_path, sizeof(game_init.default_resource_relative_path), "loco/");

	return game_init;
}

void loco_update_and_render(float delta_time, int32 window_width, int32 window_height, GameInput* input)
{
	// init scene
	static bool is_scene_init = false;
	if (!is_scene_init)
	{
		init_scene();
		is_scene_init = true;
	}

	// hot-reload resources
	resource_manager.hot_reload<Shader>();
	resource_manager.hot_reload<Material>();
	resource_manager.hot_reload<Mesh>();
	resource_manager.hot_reload<Texture>();

	// gameplay here
	camera_update(camera, world, input, delta_time);

	// update world
	world.update();

	// render world
	Viewport viewport = { 0, 0, window_width, window_height };
	defered_resources.update(window_width, window_height);
	renderer_deferred(world, camera, viewport, defered_resources);
	
	frame();

	world.gc(entity_manager);

	//log.info("FPS", "%f", 1.0f / delta_time);
}
