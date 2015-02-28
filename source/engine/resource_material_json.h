#ifndef RESOURCE_MATERIAL_JSON_H_HEADER_GUARD
#define RESOURCE_MATERIAL_JSON_H_HEADER_GUARD

#include "resource_type.h"
#include "memory_utils.h"

namespace loco
{
	namespace resource
	{
		class ResourceManager;

		bool load_material(const ResourceManager& resource_manager, const Memory* mem, MaterialData* mat, const DefaultResources& default_resources);
	} // resource
} // loco

#endif // RESOURCE_MATERIAL_JSON_H_HEADER_GUARD