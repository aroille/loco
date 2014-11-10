#ifndef WORLD_H_HEADER_GUARD
#define WORLD_H_HEADER_GUARD

#include "transform_component.h"

namespace loco
{
	class World
	{
	public:
		TransformSystem* transform_system() { return &_transform_system; }

	private:
		TransformSystem _transform_system;
	};
}

#endif // WORLD_H_HEADER_GUARD