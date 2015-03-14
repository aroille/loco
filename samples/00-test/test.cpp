
#include "loco.h"
#include "resource_manager.h"
#include "timer.h"
#include "world.h"
#include "type.h"
#include "entry.h"
#include "log.h"

#include <bx/fpumath.h>
#include "bgfx.h" //#include "bgfx_temp.h"

using namespace loco;
using namespace loco::math;

static World world;
static Entity camera;

void loco_init(int argc, char** argv, GameInit* game_init)
{
	game_init->locked_mouse = true;
	game_init->renderer_type = Renderer::Type::Count;

	strcpy_s(game_init->resource_root_path, sizeof(game_init->resource_root_path), argc > 1 ? argv[1] : "resources/");
	strcpy_s(game_init->default_resource_relative_path, sizeof(game_init->default_resource_relative_path), "loco/");
}

Entity create_axis(World& world, float length, float thickness)
{
	float l = 0.5f * length ;
	float t = 0.5f * thickness;
	float p = l + t;

	Vector3 position[4] = { { 0, 0, 0 }		, { p, 0, 0 }		, { 0, p, 0 }		, { 0, 0, p } };
	Vector3 scale[4]		= { { t, t, t }		, { l, t, t }		, { t, l, t }		, { t, t, l } };
	Vector4 color[4]		= { { 1, 1, 1, 1 }, { 1, 0, 0, 1 }, { 0, 1, 0, 1 }, { 0, 0, 1, 0 } };

	Entity origin = entity_manager.create();
	TransformComponent origin_tf = world.transform.create(origin);

	for (int i = 0; i < 4; i++)
	{
		// create entity
		Entity e = entity_manager.create();

		// create Transform component
		TransformComponent tf = world.transform.create(e);
		world.transform.link(tf, origin_tf);
		world.transform.set_local_matrix(tf, Matrix4x4{ { { scale[i].x, 0, 0, 0, 0, scale[i].y, 0, 0, 0, 0, scale[i].z, 0, position[i].x, position[i].y, position[i].z, 1 } } });

		// create MeshRender component
		MeshRenderComponent mesh_render = world.mesh_render.create(e);
		Mesh cube = resource_manager.get<Mesh>("loco/mesh/cube").duplicate();
		world.mesh_render.set_mesh(mesh_render, cube);

		// set Material
		Material mat = resource_manager.get<Material>("loco/material/uni_color").duplicate();
		mat->set("u_color", Renderer::UniformType::Vector4, (float*)&color[i]);
		cube->materials[0] = mat;
	}

	return origin;
}

Entity create_sponza(World& world)
{
	Mesh sponza_mesh = resource_manager.get<Mesh>("sponza/sponza");

	Entity sponza_entity = entity_manager.create();
	TransformComponent sponza_tf = world.transform.create(sponza_entity);
	MeshRenderComponent sponza_mr = world.mesh_render.create(sponza_entity);

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
	world.transform.set_local_matrix(axis_tf, Matrix4x4{ { { 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 0.0, 0.0, 0.5, 0.0, 0.0, 2.0, 0.0, 1.0 } } });

	// Create sponza
	resource_manager.load_folder("sponza/");
	Entity sponza_entity = create_sponza(world);

	// Create camera
	camera = entity_manager.create();
	CameraComponent camera_cp = world.camera.create(camera);
	TransformComponent camera_tf = world.transform.create(camera);

	// Set view 0 clear state.
	bgfx::setViewClear(0
		, BGFX_CLEAR_COLOR_BIT | BGFX_CLEAR_DEPTH_BIT
		, 0x101010ff
		, 1.0f
		, 0
		);
}

void camera_update(Entity camera, World& world, GameInput* input, float delta_time)
{
	float mouse_sensibility = 0.1f;
	
	float controller_move_x = input->keyboard.left_thumb.x +
														input->gamepad[0].left_thumb.x;

	float controller_move_y = input->keyboard.left_thumb.y +
														input->gamepad[0].left_thumb.y;

	float controller_rotate_x = input->mouse.abs_move.x * mouse_sensibility +
															input->keyboard.right_thumb.x +
															input->gamepad[0].right_thumb.x;

	float controller_rotate_y =-input->mouse.abs_move.y * mouse_sensibility +
															input->keyboard.right_thumb.y +
															input->gamepad[0].right_thumb.y;
															
	// init static variables
	static float cam_move_speed = 5.0f;
	static float cam_rotation_speed = 0.5f * 3.14f;

	static float fov = 60.0f;
	static float fov_speed = 20.0f;
	static Vector3 position = Vector3{ 0.0f, 0.0f, 0.0f };
	static Vector3 target = Vector3{ 0.0f, 0.0f, -35.0f };
	static Vector3 up = Vector3{ 0.0f, 1.0f, 0.0f };
	static float horizontal_angle = 0.0f;
	static float vertical_angle = 0.0f;

	static Entity cam = camera;
	static CameraComponent cam_cp = world.camera.lookup(cam);
	static TransformComponent cam_tf = world.transform.lookup(cam);

	if (cam.id != camera.id)
	{
		cam = camera;
		cam_cp = world.camera.lookup(cam);
		cam_tf = world.transform.lookup(cam);
	}

	// update
	{
		horizontal_angle += controller_rotate_x * cam_rotation_speed * delta_time;
		vertical_angle += controller_rotate_y * cam_rotation_speed * delta_time;
	}

	Vector3 direction = { cosf(vertical_angle) * sinf(horizontal_angle),
		sinf(vertical_angle),
		cosf(vertical_angle) * cosf(horizontal_angle) };

	Vector3 right = { sinf(horizontal_angle - bx::piHalf),
		0,
		cosf(horizontal_angle - bx::piHalf) };

	// move front/back
	{
		Vector3 tmp_position = position;
		Vector3 delta_position;
		bx::vec3Mul((float*)&delta_position, (float*)&direction, controller_move_y * cam_move_speed * delta_time);
		bx::vec3Add((float*)&position, (float*)&tmp_position, (float*)&delta_position);
	}

	// move left/right
	{
		Vector3 tmp_position = position;
		Vector3 delta_position;
		bx::vec3Mul((float*)&delta_position, (float*)&right, -controller_move_x * cam_move_speed * delta_time);
		bx::vec3Add((float*)&position, (float*)&tmp_position, (float*)&delta_position);
	}

	// move up/down
	{
		Vector3 tmp_position = position;
		Vector3 delta_position;
		Vector3 up_ref = { 0.0f, 1.0f, 0.0f };
		bx::vec3Mul((float*)&delta_position, (float*)&up_ref,
			(input->gamepad[0].right_trigger - input->gamepad[0].left_trigger) * cam_move_speed * delta_time);
		bx::vec3Add((float*)&position, (float*)&tmp_position, (float*)&delta_position);
	}

	bx::vec3Add((float*)&target, (float*)&position, (float*)&direction);
	bx::vec3Cross((float*)&up, (float*)&right, (float*)&direction);

	Matrix4x4 view;
	bx::mtxLookAt((float*)&view, (float*)&position, (float*)&target, (float*)&up);
	world.transform.set_local_matrix(cam_tf, view);

	// fov
	if (input->gamepad[0].left_shoulder.is_down)
	{
		fov += fov_speed * delta_time;
	}

	if (input->gamepad[0].right_shoulder.is_down)
	{
		fov -= fov_speed * delta_time;
	}

	world.camera.set_fov(cam_cp, fov);
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

	// update and render world
	world.update();

	// This dummy draw call is here to make sure that view 0 is cleared
	// if no other draw calls are submitted to view 0.
	bgfx::submit(0);
	
	Viewport viewport = { 0, 0, window_width, window_height };
	render(world, camera, viewport);
	frame();
	world.gc(entity_manager);

	//log.info("FPS", "%f", 1.0f / delta_time);
}
