#ifndef SYSTEM_COMMON_H_HEADER_GUARD
#define SYSTEM_COMMON_H_HEADER_GUARD

#include "entity.h"

namespace loco
{
	struct World;
	
	typedef math::Matrix4x4&(*CallbackTransformSync)(World*, Entity);
}
#endif // SYSTEM_COMMON_H_HEADER_GUARD
