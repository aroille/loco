#ifndef WORLD_H_HEADER_GUARD
#define WORLD_H_HEADER_GUARD

#include "transform_system.h"

namespace loco
{
	struct World
	{
		TransformSystem transforms;
	};
}

#endif // WORLD_H_HEADER_GUARD