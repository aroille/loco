
#include "loco.h"
#include "math_types.h"

using namespace loco;

int children_count(World* world, TransformComponent::Instance i)
{
	int result = 0;
	TransformComponent::Instance child = world->transform()->first_child(i);
	while (world->transform()->is_valid(child))
	{
		++result;
		child = world->transform()->next_sibling(child);
	}

	return result;
}

int main(int, char**)
{
	Game::init();

	World* world = Game::world();

	// Create entities

	Entity e_1 = Game::entity_manager()->create();
	Entity e_2 = Game::entity_manager()->create();
	Entity e_3 = Game::entity_manager()->create();
	Entity e_4 = Game::entity_manager()->create();
	Entity e_5 = Game::entity_manager()->create();

	// Add transform component to each entities

	TransformComponent::Instance tf_1 = world->transform()->create(e_1);
	TransformComponent::Instance tf_2 = world->transform()->create(e_2);
	TransformComponent::Instance tf_3 = world->transform()->create(e_3);
	TransformComponent::Instance tf_4 = world->transform()->create(e_4);
	TransformComponent::Instance tf_5 = world->transform()->create(e_5);

	world->transform()->link(tf_2, tf_1);
	world->transform()->link(tf_3, tf_2);
	world->transform()->link(tf_4, tf_2);
	world->transform()->link(tf_5, tf_2);

	world->transform()->set_local_matrix(tf_1, Matrix4x4{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1 });
	world->transform()->set_local_matrix(tf_2, Matrix4x4{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 2, 0, 0, 1 });
	world->transform()->set_local_matrix(tf_3, Matrix4x4{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 3, 0, 0, 1 });
	world->transform()->set_local_matrix(tf_4, Matrix4x4{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 4, 0, 0, 1 });
	world->transform()->set_local_matrix(tf_5, Matrix4x4{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 5, 0, 0, 1 });

	world->transform()->unlink(tf_3);

	int child_count = children_count(world, tf_2);

	Matrix4x4 tf_world_1 = world->transform()->world_matrix(tf_1);
	Matrix4x4 tf_world_2 = world->transform()->world_matrix(tf_2);
	Matrix4x4 tf_world_3 = world->transform()->world_matrix(tf_3);
	Matrix4x4 tf_world_4 = world->transform()->world_matrix(tf_4);
	Matrix4x4 tf_world_5 = world->transform()->world_matrix(tf_5);

	Game::shutdown();

	return 0;
}