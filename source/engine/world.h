#ifndef WORLD_H_HEADER_GUARD
#define WORLD_H_HEADER_GUARD

#include "transform_component.h"

namespace loco
{
	class World
	{
	public:
		TransformComponent* tf_component() { return &_tf_component; }

	private:
		TransformComponent _tf_component;
	};
}

#endif // WORLD_H_HEADER_GUARD