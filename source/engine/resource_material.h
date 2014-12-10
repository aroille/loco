#ifndef RESOURCE_MATERIAL_H_HEADER_GUARD
#define RESOURCE_MATERIAL_H_HEADER_GUARD

#include "resource_material_json.h"

namespace loco
{
	template<> MaterialPtr ResourceManager::create(const Memory* mem) const
	{
		Material* mat = load_material(mem);


		MaterialPtr mat_ptr(mat);
		return mat_ptr;
	}
	
	template<> void ResourceManager::destroy(const MaterialPtr& material) const
	{
	
	}
}

#endif // RESOURCE_MATERIAL_H_HEADER_GUARD