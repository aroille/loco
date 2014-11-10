
#include "loco.h"
#include "math_types.h"

using namespace loco;

int children_count(World* world, TransformComponent c)
{
	int result = 0;
	TransformComponent child = world->transform_system()->first_child(c);
	while (world->transform_system()->is_valid(child))
	{
		++result;
		child = world->transform_system()->next_sibling(child);
	}

	return result;
}

int main(int, char**)
{
	Game::init();

	World* world = Game::world();
	TransformSystem* tf_system = world->transform_system();

	// Create entities
	Entity e_1 = Game::create_entity();
	Entity e_2 = Game::create_entity();
	Entity e_3 = Game::create_entity();
	Entity e_4 = Game::create_entity();
	Entity e_5 = Game::create_entity();

	// Add transform component to each entities
	TransformComponent tf_1 = tf_system->create(e_1);
	TransformComponent tf_2 = tf_system->create(e_2);
	TransformComponent tf_3 = tf_system->create(e_3);
	TransformComponent tf_4 = tf_system->create(e_4);
	TransformComponent tf_5 = tf_system->create(e_5);

	// Create parent/child relations between the transform components
	tf_system->link(tf_2, tf_1);
	tf_system->link(tf_3, tf_2);
	tf_system->link(tf_4, tf_2);
	tf_system->link(tf_5, tf_2);

	// setting the local transform matrix for each transform component
	tf_system->set_local_matrix(tf_1, Matrix4x4{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1 });
	tf_system->set_local_matrix(tf_2, Matrix4x4{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 2, 0, 0, 1 });
	tf_system->set_local_matrix(tf_3, Matrix4x4{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 3, 0, 0, 1 });
	tf_system->set_local_matrix(tf_4, Matrix4x4{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 4, 0, 0, 1 });
	tf_system->set_local_matrix(tf_5, Matrix4x4{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 5, 0, 0, 1 });

	// destroy a parent/child relation
	tf_system->unlink(tf_3);

	int child_count = children_count(world, tf_2);

	// get the world transform matrix of
	Matrix4x4 tf_world_1 = tf_system->world_matrix(tf_1);
	Matrix4x4 tf_world_2 = tf_system->world_matrix(tf_2);
	Matrix4x4 tf_world_3 = tf_system->world_matrix(tf_3);
	Matrix4x4 tf_world_4 = tf_system->world_matrix(tf_4);
	Matrix4x4 tf_world_5 = tf_system->world_matrix(tf_5);

	Game::shutdown();

	return 0;
}