
#include "loco.h"
#include "math_types.h"

using namespace loco;

int main(int, char**)
{
	Game::init();

	World* world = Game::world();

	Entity e_1 = Game::entity_manager()->create();
	Entity e_2 = Game::entity_manager()->create();
	
	TransformComponent::Instance tf_1 = world->tf_component()->create(e_1);
	TransformComponent::Instance tf_2 = world->tf_component()->create(e_2);
	world->tf_component()->set_local(tf_1, Matrix4x4{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 3, 0, 0, 1 });
	world->tf_component()->link(tf_2, tf_1);
	world->tf_component()->set_local(tf_2, Matrix4x4{ 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 5, 0, 1 });

	world->tf_component()->unlink(tf_2);

	world->tf_component()->destroy(e_1);
	world->tf_component()->destroy(e_2);

	Game::entity_manager()->destroy(e_1);
	Game::entity_manager()->destroy(e_1);

	Game::shutdown();

	return 0;
}