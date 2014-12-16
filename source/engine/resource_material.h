#ifndef RESOURCE_MATERIAL_H_HEADER_GUARD
#define RESOURCE_MATERIAL_H_HEADER_GUARD

#include "resource_material_json.h"
#include "loco.h"

namespace loco
{
	template<> ResourceManager::ResourceType::Enum ResourceManager::resource_type<MaterialPtr>() const
	{ 
		return ResourceType::Material;
	}

	template<> const char* ResourceManager::resource_type_name<MaterialPtr>() const
	{
		return "Material";
	}

	template<> std::map<ResourceName, MaterialPtr>& ResourceManager::resource_map()
	{
		return _materials;
	}

	template<> MaterialPtr ResourceManager::create(const Memory* mem) const
	{
		Material* mat = new Material();
		bool load_success = load_material(mem, mat);

		if (!load_success)
		{
			LOCO_ASSERTF(ResourceManager::default_resource_init, LOCO_LOG_RESOURCE_MANAGER, "The default material is not available.");
			log.error(LOCO_LOG_RESOURCE_MANAGER, "Use of default material");
			delete mat;
			return ResourceManager::default_material.duplicate();
		}
		else
		{
			return MaterialPtr(mat);
		}
	}
	
	template<> MaterialPtr ResourceManager::replace(MaterialPtr& current, const Memory* mem) const
	{
		if (current.get() == nullptr)
			current.reset(new Material());

		bool load_success = load_material(mem, current.get());

		if (!load_success)
		{
			LOCO_ASSERTF(ResourceManager::default_resource_init, LOCO_LOG_RESOURCE_MANAGER, "The default material is not available.");
			log.error(LOCO_LOG_RESOURCE_MANAGER, "Use of default material");
			*(current.get()) = *(ResourceManager::default_material.get());
		}

		return current;
	}

	template<> void ResourceManager::destroy(const MaterialPtr& material) const
	{
	
	}
}

#endif // RESOURCE_MATERIAL_H_HEADER_GUARD