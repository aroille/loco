#ifndef RESOURCE_MATERIAL_H_HEADER_GUARD
#define RESOURCE_MATERIAL_H_HEADER_GUARD

#include "resource_material_json.h"

namespace loco
{
	template<> std::map<ResourceName, MaterialPtr>& ResourceManager::resource_map()	
	{ 
		return _materials; 
	}

	template<> MaterialPtr ResourceManager::create(const Memory* mem) const
	{
		Material* mat = new Material();
		load_material(mem, mat);

		MaterialPtr mat_ptr(mat);
		return mat_ptr;
	}
	
	template<> MaterialPtr ResourceManager::replace(MaterialPtr& current, const Memory* mem) const
	{
		load_material(mem, current.get());
		return current;
	}

	template<> void ResourceManager::destroy(const MaterialPtr& material) const
	{
	
	}
}

#endif // RESOURCE_MATERIAL_H_HEADER_GUARD