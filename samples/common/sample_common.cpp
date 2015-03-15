
#include "sample_common.h"
#include "resource_manager.h"
#include <bx/fpumath.h>

using namespace loco;
using namespace loco::math;

Entity create_axis(World& world, float length, float thickness)
{
	float l = 0.5f * length;
	float t = 0.5f * thickness;
	float p = l + t;

	Vector3 position[4] = { { 0, 0, 0 }, { p, 0, 0 }, { 0, p, 0 }, { 0, 0, p } };
	Vector3 scale[4] = { { t, t, t }, { l, t, t }, { t, l, t }, { t, t, l } };
	Vector4 color[4] = { { 1, 1, 1, 1 }, { 1, 0, 0, 1 }, { 0, 1, 0, 1 }, { 0, 0, 1, 0 } };

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

void camera_update(Entity camera, World& world, GameInput* input, float delta_time)
{
	// init static variables
	static float cam_move_speed = 5.0f;
	static float cam_rotation_speed = 0.5f * 3.14f;

	static float fov = 60.0f;
	static float fov_speed = 20.0f;
	static Vector3 position = Vector3(0.0f, 0.0f, 0.0f);
	static Vector3 target = Vector3(0.0f, 0.0f, -35.0f);
	static Vector3 up = Vector3(0.0f, 1.0f, 0.0f);
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

	// get controller value
	float mouse_sensibility = 0.1f;

	float controller_move_x = input->keyboard.left_thumb.x +
		input->gamepad[0].left_thumb.x;

	float controller_move_y = input->gamepad[0].right_trigger - input->gamepad[0].left_trigger;

	float controller_move_z = input->keyboard.left_thumb.y +
		input->gamepad[0].left_thumb.y;

	float controller_rotate_x = input->mouse.abs_move.x * mouse_sensibility +
		input->keyboard.right_thumb.x +
		input->gamepad[0].right_thumb.x;

	float controller_rotate_y = -input->mouse.abs_move.y * mouse_sensibility +
		input->keyboard.right_thumb.y +
		input->gamepad[0].right_thumb.y;

	// rotation
	horizontal_angle += controller_rotate_x * cam_rotation_speed * delta_time;
	vertical_angle += controller_rotate_y * cam_rotation_speed * delta_time;

	// translation
	Vector3 direction(cosf(vertical_angle) * sinf(horizontal_angle),
		sinf(vertical_angle),
		cosf(vertical_angle) * cosf(horizontal_angle));

	Vector3 right(sinf(horizontal_angle - HALF_PI),
		0,
		cosf(horizontal_angle - HALF_PI));

	position += direction * (controller_move_z * cam_move_speed * delta_time);
	position += right * (-controller_move_x * cam_move_speed * delta_time);
	position += Vector3(0.0, 1.0, 0.0) * (controller_move_y * cam_move_speed * delta_time);

	// set camera local matrix
	target = position + direction;
	up = Vector3::cross(right, direction);

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


