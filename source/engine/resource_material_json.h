#ifndef RESOURCE_MATERIAL_JSON_H_HEADER_GUARD
#define RESOURCE_MATERIAL_JSON_H_HEADER_GUARD

#include "resource_type.h"
#include "memory_utils.h"
namespace loco
{
	namespace resource
	{
		bool load_material(const Memory* mem, MaterialData* mat, const DefaultResources& default_resources);
	} // resource
} // loco

#endif // RESOURCE_MATERIAL_JSON_H_HEADER_GUARD