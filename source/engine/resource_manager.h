#ifndef RESOURCE_MANAGER_H_HEADER_GUARD
#define RESOURCE_MANAGER_H_HEADER_GUARD

#include "resource_type.h"
#include "debug.h"
#include "murmur_hash.h"
#include "file_utils.h"

#include <vector>
#include <map>

namespace loco
{
	struct Memory;
	
	typedef HashedString ResourceName;

	class ResourceManager
	{
	public:

		struct ResourceType
		{
			enum Enum
			{
				Texture,
				Shader,
				Material,
				Mesh,
				Count,

				Unknown,
			};
		};

		struct ResourceInfo
		{
			ResourceName		name;
			ResourceType::Enum	type;
			FileInfo			file_info;
		};

		ResourceManager();
		~ResourceManager();

		/// Load all resources in path (recursive)
		/// Return the number of resource loaded by the function call
		unsigned load_folder(const char* folder_path);

		/// Return the resource associated with a specific resource path and resource type
		template<typename T> T get(const char* resource_path)
		{
			return get<T>(resource_name(resource_path), resource_path);
		}

		/// Return the resource associated with a specific resource handle and resource type
		template<typename T> T get(ResourceName name, const char* debug_name = nullptr)
		{
			std::map<HashedString, T>& map = resource_map<T>();

			auto it = map.find(name);
			if (it != map.end())
			{
				return it->second;
			}
			else
			{
				LOCO_ASSERTF(false, "Resource file not found : %s", debug_name != nullptr ? debug_name : "");
				//return T::invalid;
				return T();
			}		
		}

		/// Return the handle corresponding to a specific resource_path
		/// Note : The handle is unique for a specific Resource Type.
		static ResourceName get_name(const char* resource_path)
		{
			return resource_name(resource_path);
		}

		/// Unload all resources holded by this resource manager
		void unload_all();

		/// Reload all modified resources of type T
		template<typename T> void hot_reload()
		{
			ResourceType::Enum type = ResourceType::Material;
			for (auto it = _resources[type].begin(); it != _resources[type].end(); it++)
			{
				unsigned long long modif_date = file_modification_date(it->second.path);
				//if (modif_date > it->second.last_modif_date)
				{
					/// WARNING : memory leak here
					const Memory* mem = file_read(it->second);
					resource_map<T>()[it->first] = replace<T>(resource_map<T>()[it->first], mem);

					it->second.last_modif_date = modif_date;
				}
			}
		}


	private:

		std::map<ResourceName, FileInfo>		_resources[ResourceType::Count];

		std::map<ResourceName, MaterialPtr>		_materials;
		std::map<ResourceName, Mesh>			_meshes;
		std::map<ResourceName, Shader>			_shaders;
		std::map<ResourceName, Texture>			_textures;

		// ------
		bool load_resource(const ResourceInfo& ri);
		void create_resource(const ResourceInfo& ri, const Memory* mem);
		void replace_resource(const ResourceInfo& ri, const Memory* mem);
		void destroy_resource(const ResourceInfo& ri);
		
		static ResourceName resource_name(const char* resource_path);
		static ResourceName resource_name(const FileInfo& fi);
		static ResourceType::Enum resource_type(const FileInfo& fi);

		template<typename T> void create_resource(const ResourceInfo& ri, const Memory* mem)
		{
			resource_map<T>()[ri.name] = create<T>(mem);
		}

		template<typename T> void replace_resource(const ResourceInfo& ri, const Memory* mem)
		{
			resource_map<T>()[ri.name] = replace<T>(resource_map<T>()[ri.name], mem);
		}

		template<typename T> void destroy_resource(const ResourceInfo& ri)
		{
			destroy<T>(resource_map<T>()[ri.name]);
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

		/// function to specialize per resource type
		template<typename T> std::map<ResourceName, T>& resource_map();
		template<typename T> T create(const Memory* mem) const;
		template<typename T> T replace(T& current, const Memory* replace) const;
		template<typename T> void destroy(const T& resource) const;
	};
}

#endif // RESOURCE_MANAGER_H_HEADER_GUARD