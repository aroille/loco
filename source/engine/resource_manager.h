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

		struct ResourceId
		{
			ResourceType::Enum	type;
			ResourceName		name;

			bool operator<(ResourceId const& in) const
			{
				return (type < in.type) ? true : ((type > in.type) ? false : (name < in.name));
			}
		};

		struct ResourceInfo
		{
			ResourceId			id;
			FileInfo			file_info;
		};

		~ResourceManager();

		/// Load all resources in path (recursive)
		/// Return the number of resource loaded by the function call
		unsigned load_folder(const char* folder_path);

		bool unload_folder(const char* folder_path);

		/// Unload all resources holded by this resource manager
		void unload_all();

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
				log.error(LOCO_LOG_RESOURCE_MANAGER, "%s not found : %s", resource_type_name<T>(), debug_name ? debug_name : "");
				return T::invalid;
			}		
		}

		/// Return the handle corresponding to a specific resource_path
		/// Note : The handle is unique for a specific Resource Type.
		static ResourceName get_name(const char* resource_path)
		{
			return resource_name(resource_path);
		}

		/// Reload all modified resources of type T
		template<typename T> void hot_reload()
		{
			for (auto folder_it = _files.begin(); folder_it != _files.end(); folder_it++)
			{
				const HashedString& folder_path = folder_it->first;

				ResourceType::Enum type = resource_type<T>();
				for (auto it = _files[folder_path].begin(); it != _files[folder_path].end(); it++)
				{
					const ResourceId& id = it->first;
					FileInfo& file_info = it->second;

					if (id.type != type)
						continue;
					
					unsigned long long modif_date = file_modification_date(file_info.path);
					if (modif_date == 0 && modif_date < file_info.last_modif_date)
					{
						log.warning(LOCO_LOG_RESOURCE_MANAGER, "Hot reloading, the file no longer exists %s", file_info.path);
						file_info.last_modif_date = modif_date;
					}
					else if (modif_date > file_info.last_modif_date)
					{
						log.info(LOCO_LOG_RESOURCE_MANAGER, "Hot reloading %s", file_info.path);
						const Memory* old_mem = file_info.mem;
						
						bool read_success = file_read(file_info);
						if (read_success)
						{
							release(old_mem);
							resource_map<T>()[id.name] = replace<T>(resource_map<T>()[id.name], file_info.mem);
						}
						file_info.last_modif_date = modif_date;
					}
				}
			}
		}

		static void init_default_resources();

		static bool			default_resource_init;

		static Shader		default_vertex_shader;
		static Shader		default_pixel_shader;
		static Texture		default_texture;
		static Mesh			default_mesh;
		static MaterialPtr	default_material;

	private:

		typedef std::map<ResourceId, FileInfo> FileMap;
		std::map<HashedString, FileMap>			_files;

		std::map<ResourceName, MaterialPtr>		_materials;
		std::map<ResourceName, Mesh>			_meshes;
		std::map<ResourceName, Shader>			_shaders;
		std::map<ResourceName, Texture>			_textures;

		// ------

		bool load_resource(ResourceInfo& ri, const HashedString& root_folder);
		bool unload_resource(const ResourceId& id, const HashedString& root_folder);
		bool unload_folder(const HashedString& folder_path);

		void create_resource(const ResourceId& id, const Memory* mem);
		void replace_resource(const ResourceId& id, const Memory* mem);
		void destroy_resource(const ResourceId& id);
		
		static ResourceName resource_name(const char* resource_path);
		static ResourceId resource_id(const FileInfo& fi);

		template<typename T> void create_resource(const ResourceId& id, const Memory* mem)
		{
			resource_map<T>()[id.name] = create<T>(mem);
		}

		template<typename T> void replace_resource(const ResourceId& id, const Memory* mem)
		{
			resource_map<T>()[id.name] = replace<T>(resource_map<T>()[id.name], mem);
		}

		template<typename T> void destroy_resource(const ResourceId& id)
		{
			destroy<T>(resource_map<T>()[id.name]);
			resource_map<T>().erase(id.name);
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
		template<typename T> ResourceType::Enum resource_type() const;
		template<typename T> const char* resource_type_name() const;
		template<typename T> std::map<ResourceName, T>& resource_map();
		template<typename T> T create(const Memory* mem) const;
		template<typename T> T replace(T& current, const Memory* replace) const;
		template<typename T> void destroy(const T& resource) const;
	};
}

#endif // RESOURCE_MANAGER_H_HEADER_GUARD