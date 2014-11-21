#ifndef RESOURCE_MANAGER_H_HEADER_GUARD
#define RESOURCE_MANAGER_H_HEADER_GUARD

#include "murmur_hash.h"
#include "renderer.h"
#include <map>

namespace loco
{
	struct FileInfo;
	struct Memory;

	class ResourceManager
	{
	public:

		struct ResourceType
		{
			enum Enum
			{
				Texture,

				Count,
			};
		};

		struct ResourceInfo
		{
			HashedString		name;
			ResourceType::Enum	type;
			unsigned long long	file_date;
		};

		struct Mesh
		{
			renderer::IndexBufferHandle vertex_buffer;
			renderer::IndexBufferHandle index_buffer;
		};

		struct Texture
		{
			renderer::TextureHandle handle;

			static const Texture invalid;
		};

		//struct Texture	{ HashedString name; };
		//struct Mesh		{ HashedString name; };
		//struct Shader	{ HashedString name; };
		//struct Program	{ HashedString name; };

		~ResourceManager();

		/// Set the root folder path of this resource manager
		void init(const char* root_folder_path);

		/// Load all resources in path (recursive) + create the equivalent gpu resources
		/// Return the number of resource loaded by the function call
		unsigned load_folder(const char* folder_path);

		/// Return the resource associate with a specific resource path and resource type
		template<typename T> T get(const char* resource_path)
		{
			HashedString name = resource_name(resource_path);
			std::map<HashedString, T>& m = resource_map<T>();

			auto it = m.find(name);
			if (it != m.end())
				return it->second;
			else
				return T::invalid;
		};

		void unload_all();

	private:

		char		_root_path[LOCO_PATH_LENGTH];
		unsigned	_root_path_lenght;
		std::map<HashedString, ResourceInfo>	_resources[ResourceType::Count];
		std::map<HashedString, Texture>			_textures;
		// ------
		bool load_file(const FileInfo& fi);
		void create_resource(const ResourceInfo& ri, const Memory* mem);
		
		HashedString resource_name(const FileInfo& fi) const;
		HashedString resource_name(const char* resource_path) const;
		ResourceType::Enum resource_type(const FileInfo& fi) const;
		
		template<typename T> std::map<HashedString, T>& resource_map();
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

		template<typename T> void test();
	};
}

#endif // RESOURCE_MANAGER_H_HEADER_GUARD