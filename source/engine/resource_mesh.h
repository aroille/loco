#ifndef RESOURCE_MESH_H_HEADER_GUARD
#define RESOURCE_MESH_H_HEADER_GUARD

#include "resource_mesh_bgfx.h"

namespace loco
{
	template<> ResourceManager::ResourceType::Enum ResourceManager::resource_type<Mesh>() const
	{
		return ResourceType::Mesh;
	}

	template<> const char* ResourceManager::resource_type_name<Mesh>() const
	{
		return "Mesh";
	}

	template<> std::map<ResourceName, Mesh>& ResourceManager::resource_map()
	{
		return _meshes;
	}

	template<> Mesh ResourceManager::create(const Memory* mem) const
	{
		MeshData* mesh = new MeshData();
		bool load_success = bgfx_helper::load_mesh(mem, mesh);

		if (!load_success)
		{
			LOCO_ASSERTF(ResourceManager::default_resource_init, LOCO_LOG_RESOURCE_MANAGER, "The default mesh is not available.");
			log.error(LOCO_LOG_RESOURCE_MANAGER, "Use of default mesh");
			delete mesh;
			return ResourceManager::default_mesh.duplicate();
		}
		else
		{
			return Mesh(mesh);
		}
		
	}

	template<> Mesh ResourceManager::replace(Mesh& current, const Memory* mem) const
	{
		if (current.get() == nullptr)
			current.reset(new MeshData());


		bool load_success = bgfx_helper::load_mesh(mem, current.get());

		if (!load_success)
		{
			LOCO_ASSERTF(ResourceManager::default_resource_init, LOCO_LOG_RESOURCE_MANAGER, "The default mesh is not available.");
			log.error(LOCO_LOG_RESOURCE_MANAGER, "Use of default mesh");
			*(current.get()) = *(ResourceManager::default_mesh.get());
		}

		return current;
	}

	template<> void ResourceManager::destroy(const Mesh& mesh) const
	{

	}
}

#endif // RESOURCE_MESH_H_HEADER_GUARD