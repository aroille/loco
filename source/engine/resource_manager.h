#ifndef RESOURCE_MANAGER_H_HEADER_GUARD
#define RESOURCE_MANAGER_H_HEADER_GUARD

#include "murmur_hash.h"
#include "renderer.h"

#include <vector>
#include <map>

namespace loco
{
	struct FileInfo;
	struct Memory;
	class Material;

	//
	typedef HashedString ResourceName;

	typedef renderer::TextureHandle Texture;
	typedef renderer::ShaderHandle Shader;

	struct Mesh
	{
		std::vector<Material*> materials;
		std::vector<renderer::VertexBufferHandle> vertex_buffers;
		std::vector<renderer::IndexBufferHandle> index_buffers;
	};

	class ResourceManager
	{
	public:

		struct ResourceType
		{
			enum Enum
			{
				Mesh,
				Shader,
				Texture,
				Count,
			};
		};

		struct ResourceInfo
		{
			ResourceName		name;
			ResourceType::Enum	type;
			unsigned long long	file_date;
		};

		~ResourceManager();

		/// Load all resources in path (recursive)
		/// Return the number of resource loaded by the function call
		unsigned load_folder(const char* folder_path);

		/// Return the resource associated with a specific resource path and resource type
		template<typename T> T get(const char* resource_path)
		{
			return get<T>(resource_name(resource_path));
		}

		/// Return the resource associated with a specific resource handle and resource type
		template<typename T> T get(ResourceName name)
		{
			std::map<HashedString, T>& map = resource_map<T>();

			auto it = map.find(name);
			if (it != map.end())
				return it->second;
			else
				return T::invalid;
		}

		/// Return the handle corresponding to a specific resource_path
		/// Note : The handle is unique for a specific Resource Type.
		static ResourceName get_name(const char* resource_path)
		{
			return resource_name(resource_path);
		}

		/// Unload all resources holded by this resource manager
		void unload_all();

	private:

		std::map<ResourceName, ResourceInfo>	_resources[ResourceType::Count];
		std::map<ResourceName, Mesh>			_meshes;
		std::map<ResourceName, Shader>			_shaders;
		std::map<ResourceName, Texture>			_textures;

		// ------
		bool load_file(const FileInfo& fi);
		void create_resource(const ResourceInfo& ri, const Memory* mem);
		
		ResourceName resource_name(const FileInfo& fi) const;
		static ResourceName resource_name(const char* resource_path);
		ResourceType::Enum resource_type(const FileInfo& fi) const;
		
		template<typename T> std::map<ResourceName, T>& resource_map();
		template<typename T> T create(const Memory* mem) const;
		template<typename T> void destroy(const T& resource) const;

		template<typename T> void create_resource(const ResourceInfo& ri, const Memory* mem)
		{
			resource_map<T>()[ri.name] = create<T>(mem);
		}

		template<typename T> void unload_all()
		{
			auto it = resource_map<T>().begin();
			while (it != resource_map<T>().end())
			{
				destroy(it->second);
				++it;
			}
			resource_map<T>().clear();
		}
	};
}

#endif // RESOURCE_MANAGER_H_HEADER_GUARD