#ifndef RESOURCE_MATERIAL_H_HEADER_GUARD
#define RESOURCE_MATERIAL_H_HEADER_GUARD

#include "resource_material_json.h"
#include "loco.h"

namespace loco
{
namespace resource
{

	template<> ResourceManager::ResourceType::Enum ResourceManager::resource_type<Material>() const
	{
		return ResourceType::Material;
	}

	template<> const char* ResourceManager::resource_type_name<Material>() const
	{
		return "Material";
	}

	template<> std::map<ResourceName, Material>& ResourceManager::resource_map()
	{
		return _materials;
	}

	template<> Material ResourceManager::create(const Memory* mem) const
	{
		MaterialData* mat = new MaterialData();
		bool load_success = load_material(mem, mat, _default_resources);

		if (!load_success)
		{
			LOCO_ASSERTF(_default_resources_init, LOCO_RESOURCE_MANAGER, "The default material is not available.");
			log.error(LOCO_RESOURCE_MANAGER, "Use of default material");
			delete mat;
			return _default_resources.material.duplicate();
		}
		else
		{
			return Material(mat);
		}
	}

	template<> Material ResourceManager::replace(Material& current, const Memory* mem) const
	{
		if (current.get() == nullptr)
			current.reset(new MaterialData());

		bool load_success = load_material(mem, current.get(), _default_resources);

		if (!load_success)
		{
			LOCO_ASSERTF(_default_resources_init, LOCO_RESOURCE_MANAGER, "The default material is not available.");
			log.error(LOCO_RESOURCE_MANAGER, "Use of default material");
			*(current.get()) = *(_default_resources.material.get());
		}

		return current;
	}

	template<> void ResourceManager::destroy(const Material& material) const
	{

	}
} // resource
} // loco

#endif // RESOURCE_MATERIAL_H_HEADER_GUARD