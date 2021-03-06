#ifndef RESOURCE_MANAGER_H_HEADER_GUARD
#define RESOURCE_MANAGER_H_HEADER_GUARD

#include "loco.h" // renderer

#include "debug.h"
#include "murmur_hash.h"
#include "file_utils.h"
#include "log.h"

#include <queue>
#include <map>

#include "resource_type.h"

#define LOCO_RESOURCE_MANAGER "ResourceManager" // log module string

namespace loco{

	struct Memory;

	namespace resource{

	/// ResourceName
	/// (hashed filepath (without extention) to the resource)
	typedef HashedString ResourceName;

	/// LOCO Resource Manager
	class ResourceManager
	{
	public:

		/// Resource type enumeration
		enum class ResourceType
		{
			Texture,
			Shader,
			Material,
			Mesh,
			Count,

			Unknown,
		};

		/// Unique identifier of a resource
		struct ResourceId
		{
			ResourceType		type;
			ResourceName		name;

			/// Use to sort resources by type during loading
			bool operator<(ResourceId const& in) const
			{
				return (type < in.type) ? true : ((type > in.type) ? false : (name < in.name));
			}
		};

		/// Resource informations used during loading
		struct ResourceInfo
		{
			ResourceId			id;
			FileInfo			file_info;
		};

		ResourceManager();
		~ResourceManager();

		/// Load all resources in path (recursive)
		/// Return the number of resource loaded by the function call
		unsigned load_folder(const char* folder_path);

		bool unload_folder(const char* folder_path);

		/// Unload all resources holded by this resource manager
		void unload_all();

		/// Return the resource associated with a specific resource path and resource type
		template<typename T> T get(const char* resource_path) const; 

		/// Return the resource associated with a specific resource handle and resource type
		template<typename T> T get(ResourceName name, const char* debug_name = nullptr) const;

		/// Return the handle corresponding to a specific resource_path
		/// Note : The handle is unique for a specific Resource Type.
		static ResourceName get_name(const char* resource_path)
		{
			return resource_name(resource_path);
		}

		/// Reload all modified resources of type T
		template<typename T> void hot_reload();

		/// Init default resources
		/// @remarks
		/// call this function after loading the default resource folder
		void init_default_resources();

		/// Return the default resources
		const DefaultResources& get_default() const
		{
			return _default_resources;
		}

		/// Free useless buffer 
		/// To create a resource, the render backend need the buffer data for at least 2 frames.
		/// The buffer can be deleted after this delay
		void free_memory();

	private:

		bool _default_resources_init;
		DefaultResources	_default_resources;

		typedef std::map<ResourceId, FileInfo> FileMap;
		std::map<HashedString, FileMap>			_files;

		std::map<ResourceName, Material>		_materials;
		std::map<ResourceName, Mesh>				_meshes;
		std::map<ResourceName, Shader>			_shaders;
		std::map<ResourceName, Texture>			_textures;

		struct MemoryToFree
		{
			const Memory* mem;
			uint32 frame;
		};
		std::queue<MemoryToFree> _memory_to_free;

		// ------

		bool load_resource(ResourceInfo& ri, const HashedString& root_folder);
		bool unload_resource(const ResourceId& id, const HashedString& root_folder);
		bool unload_folder(const HashedString& folder_path);

		void create_resource(const ResourceId& id, const Memory* mem);
		void destroy_resource(const ResourceId& id);

		static ResourceName resource_name(const char* resource_path);
		static ResourceId resource_id(const FileInfo& fi);

		template<typename T> void create_resource(const ResourceId& id, const Memory* mem);
		template<typename T> void destroy_resource(const ResourceId& id);
		template<typename T> void unload_all();

		/// methods to specialize per resource type
		template<typename T> std::map<ResourceName, T>& resource_map();
		template<typename T> const std::map<ResourceName, T>& resource_map() const;
		template<typename T> ResourceType resource_type() const;
		template<typename T> const char* resource_type_name() const;
		template<typename T> T create(const Memory* mem) const;
		template<typename T> T replace(T& current, const Memory* replace) const;
		template<typename T> void destroy(const T& resource) const;
	};



	//==========================================================================
	template<typename T> 
	T ResourceManager::get(ResourceName name, const char* debug_name) const
	{
		const std::map<HashedString, T>& map = resource_map<T>();

		auto it = map.find(name);
		if (it != map.end())
		{
			return it->second;
		}
		else
		{
			LOCO_LOG_ERROR(LOCO_RESOURCE_MANAGER, "%s not found : %s", resource_type_name<T>(), debug_name ? debug_name : "");
			return T::invalid;
		}
	};

	//==========================================================================
	template<typename T> 
	void ResourceManager::hot_reload()
	{
#ifdef LOCO_USE_HOT_RELOAD
		for (auto folder_it = _files.begin(); folder_it != _files.end(); folder_it++)
		{
			const HashedString& folder_path = folder_it->first;

			ResourceType type = resource_type<T>();
			for (auto it = _files[folder_path].begin(); it != _files[folder_path].end(); it++)
			{
				const ResourceId& id = it->first;
				FileInfo& file_info = it->second;

				if (id.type != type)
					continue;

				unsigned long long modif_date = file_modification_date(file_info.path);
				if (modif_date == 0 && modif_date < file_info.last_modif_date)
				{
					LOCO_LOG_WARNING(LOCO_RESOURCE_MANAGER, "Hot reloading, the file no longer exists %s", file_info.path);
					file_info.last_modif_date = modif_date;
				}
				else if (modif_date > file_info.last_modif_date)
				{
					LOCO_LOG_INFO(LOCO_RESOURCE_MANAGER, "Hot reloading %s", file_info.path);

					bool read_success = file_read(file_info);
					if (read_success)
					{
						resource_map<T>()[id.name] = replace<T>(resource_map<T>()[id.name], file_info.mem);
						_memory_to_free.push(MemoryToFree{ file_info.mem, loco::current_frame });
					}
					file_info.last_modif_date = modif_date;
				}
			}
		}
#endif // LOCO_USE_HOT_RELOAD
	}

	//==========================================================================
	template<typename T> 
	T ResourceManager::get(const char* resource_path) const
	{
		return get<T>(resource_name(resource_path), resource_path);
	};

	//==========================================================================
	template<typename T> 
	void ResourceManager::create_resource(const ResourceId& id, const Memory* mem)
	{
		resource_map<T>()[id.name] = create<T>(mem);
	};

	//==========================================================================
	template<typename T> 
	void ResourceManager::destroy_resource(const ResourceId& id)
	{
		destroy<T>(resource_map<T>()[id.name]);
		resource_map<T>().erase(id.name);
	};

	//==========================================================================
	template<typename T> 
	void ResourceManager::unload_all()
	{
		auto it = resource_map<T>().begin();
		while (it != resource_map<T>().end())
		{
			destroy(it->second);
			++it;
		}
		resource_map<T>().clear();
	};

	} // resource
} // loco

#endif // RESOURCE_MANAGER_H_HEADER_GUARD