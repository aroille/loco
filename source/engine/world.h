#ifndef WORLD_H_HEADER_GUARD
#define WORLD_H_HEADER_GUARD

#include "transform_component.h"

namespace loco
{
	struct World
	{
		TransformSystem transforms;
	};
}

#endif // WORLD_H_HEADER_GUARD