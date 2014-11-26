#ifndef RESOURCE_MESH_H_HEADER_GUARD
#define RESOURCE_MESH_H_HEADER_GUARD

namespace loco
{


	template<> Mesh ResourceManager::create(const Memory* mem) const
	{
		Mesh mesh;
		//mesh.index_buffer = renderer::IndexBufferHandle{ 0 };
		//mesh.vertex_buffer = renderer::VertexBufferHandle{ 0 };
		return mesh;
	}

	template<> void ResourceManager::destroy(const Mesh& mesh) const
	{
		//renderer::destroy_texture(texture.handle);
	}
}

#endif // RESOURCE_MESH_H_HEADER_GUARD