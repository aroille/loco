#ifndef WORLD_H_HEADER_GUARD
#define WORLD_H_HEADER_GUARD

#include "transform_component.h"

namespace loco
{
	class World
	{
	public:
		TransformComponent* transform() { return &_transform; }

	private:
		TransformComponent _transform;
	};
}

#endif // WORLD_H_HEADER_GUARD