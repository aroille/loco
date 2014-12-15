#ifndef RESOURCE_MATERIAL_H_HEADER_GUARD
#define RESOURCE_MATERIAL_H_HEADER_GUARD

#include "resource_material_json.h"

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
			delete mat;
			return MaterialPtr::invalid;
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
			current.reset();
			return MaterialPtr::invalid;
		}
		else
		{
			return current;
		}
	}

	template<> void ResourceManager::destroy(const MaterialPtr& material) const
	{
	
	}
}

#endif // RESOURCE_MATERIAL_H_HEADER_GUARD