#ifndef RESOURCE_MESH_H_HEADER_GUARD
#define RESOURCE_MESH_H_HEADER_GUARD

#include "resource_mesh_bgfx.h"

namespace loco
{
	template<> ResourceManager::ResourceType::Enum ResourceManager::resource_type<Mesh>() const
	{
		return ResourceType::Mesh;
	}

	template<> std::map<ResourceName, Mesh>& ResourceManager::resource_map()
	{
		return _meshes;
	}

	template<> Mesh ResourceManager::create(const Memory* mem) const
	{
		return bgfx_helper::load_mesh(mem);
	}

	template<> Mesh ResourceManager::replace(Mesh& current, const Memory* mem) const
	{
		return current;
	}

	template<> void ResourceManager::destroy(const Mesh& mesh) const
	{
		for (unsigned i = 0; i < mesh.submeshes.size(); i++)
		{
			renderer::destroy_index_buffer(mesh.submeshes[i].index_buffer);
			renderer::destroy_vertex_buffer(mesh.submeshes[i].vertex_buffer);
		}
	}
}

#endif // RESOURCE_MESH_H_HEADER_GUARD