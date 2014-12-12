#include "resource_manager.h"

#include "resource_material.h"
#include "resource_mesh.h"
#include "resource_shader.h"
#include "resource_texture.h"

#include "debug.h"
#include "defines.h"
#include "file_system.h"
#include "file_utils.h"
#include "loco.h"
#include "memory_utils.h"

#include <string>
#include <list>
#include <map>

namespace loco
{
	//==========================================================================
	bool compare_resource_type(const ResourceManager::ResourceInfo& first, const ResourceManager::ResourceInfo& second)
	{
		return (first.id.type < second.id.type);
	}

	ResourceManager::~ResourceManager()
	{
		unload_all();
	}

	//==========================================================================
	unsigned ResourceManager::load_folder(const char* folder_path)
	{
		// check if this folder is already loaded
		HashedString root_folder = hash_string(folder_path);
		auto fi = _files.find(root_folder);
		if ((fi != _files.cend()))
		{
			LOCO_ASSERTF(false, "Loading of a folder already loaded : %s", folder_path);
			return 0;
		}

		_files[root_folder] = new FileMap[ResourceType::Count];

		// concat root path + folder path
		char path[LOCO_PATH_LENGTH];
		strcpy(path, loco::resource_root_path);
		strcat(path, folder_path);

		// get files in the folder
		std::list<FileInfo> files_infos;
		files_in_directory(path, true, &files_infos);

		// create resource list
		std::list<ResourceInfo> resources;
		for (auto fi = files_infos.begin(); fi != files_infos.end(); fi++)
		{
			// check the resource type
			ResourceType::Enum type = resource_type(*fi);
			if (type == ResourceType::Count)
				continue;

			// check if the file is already loaded
			ResourceName res_name = resource_name(*fi);
			/*
			auto res = _resources[type].find(res_name);
			if (res != _resources[type].end())
				continue;
			*/

			// ResourceInfo instance
			ResourceInfo ri;
			ri.id.name = res_name;
			ri.id.type = type;
			ri.file_info = *fi;

			resources.push_back(ri);
		}

		// sort by resource type 
		resources.sort(compare_resource_type);

		// load resources
		unsigned loaded_count = 0;
		auto res = resources.begin();
		while (res != resources.end())
		{
			loaded_count += load_resource(*res, root_folder);
			res++;
		}

		return loaded_count;
	}

	//==========================================================================
	bool ResourceManager::unload_folder(const char* folder_path)
	{
		HashedString root_folder = hash_string(folder_path);
		return unload_folder(root_folder);
	}

	//==========================================================================
	bool ResourceManager::unload_folder(const HashedString& folder_path)
	{
		// check if this folder is already loaded
		
		auto folder_files_it = _files.find(folder_path);
		if ((folder_files_it == _files.cend()))
		{
			LOCO_ASSERTF(false, "Unloading of a folder not loaded : %s", folder_path);
			return false;
		}

		FileMap* file_maps = folder_files_it->second;
		for (unsigned i = 0; i < ResourceType::Count; i++)
		{
			auto file_it = file_maps[i].begin();
			while (file_it != file_maps[i].end())
			{
				unload_resource(ResourceId{ file_it->first, (ResourceType::Enum)i }, folder_path);
				file_it = file_maps[i].begin();
			}
		}

		delete[] folder_files_it->second;
		_files.erase(folder_files_it);

		return true;
	}

	//==========================================================================
	bool ResourceManager::load_resource(const ResourceInfo& ri, const HashedString& root_folder)
	{
		// if resource already loaded, unload previous resource version
		auto fi = _files[root_folder][ri.id.type].find(ri.id.name);
		if ((fi != _files[root_folder][ri.id.type].end()))
		{
			LOCO_ASSERTF(false, "The following resource is already loaded : %s", ri.file_info.path);
			return false;
		}

		/// WARNING : memory leak here
		const Memory* mem = file_read(ri.file_info);
		create_resource(ri.id, mem);
		_files[root_folder][ri.id.type][ri.id.name] = ri.file_info;

		return true;
	}

	//==========================================================================
	bool ResourceManager::unload_resource(const ResourceId& id, const HashedString& root_folder)
	{
		// check if the resource is loaded
		auto fi = _files[root_folder][id.type].find(id.name);
		if ((fi == _files[root_folder][id.type].end()))
		{
			LOCO_ASSERTF(false, "Unloading a resource not loaded : (%d,&d,%d)", id.name, id.type, root_folder);
			return false;
		}

		destroy_resource(id);
		_files[root_folder][id.type].erase(id.name);

		return true;
	}

	//==========================================================================
	ResourceName ResourceManager::resource_name(const char* resource_path)
	{
		return hash_string(resource_path);
	}

	//==========================================================================
	ResourceName ResourceManager::resource_name(const FileInfo& fi)
	{
		unsigned path_length = strlen(fi.path);
		unsigned ext_length = strlen(fi.extention);
		unsigned size_to_hash = path_length - resource_root_path_length - ext_length - 1;
		return murmur_hash_64(fi.path + resource_root_path_length, size_to_hash);
	}

	//==========================================================================
	void ResourceManager::unload_all()
	{
		while (_files.begin() != _files.end())
		{
			unload_folder(_files.begin()->first);
		}
	}

	//==========================================================================
	ResourceManager::ResourceType::Enum ResourceManager::resource_type(const FileInfo& fi)
	{
		if (strcmp(fi.extention, "dds") == 0)
			return ResourceManager::ResourceType::Texture;
		else if (strcmp(fi.extention, "mesh") == 0)
			return ResourceManager::ResourceType::Mesh;
		else if (strcmp(fi.extention, "material") == 0)
			return ResourceManager::ResourceType::Material;
		else if (strcmp(fi.extention, "shader") == 0)
			return ResourceManager::ResourceType::Shader;
		else
			return ResourceManager::ResourceType::Count;
	}

	//==========================================================================
	void ResourceManager::create_resource(const ResourceId& id, const Memory* mem)
	{
		switch (id.type)
		{
		case ResourceType::Mesh:
			create_resource<Mesh>(id, mem);
			break;

		case ResourceType::Texture:
			create_resource<Texture>(id, mem);
			break;

		case ResourceType::Material:
			create_resource<MaterialPtr>(id, mem);
			break;

		case ResourceType::Shader:
			create_resource<Shader>(id, mem);
			break;

		default:
			LOCO_ASSERTF(false, "Resources of type %d are not handled by the resource manager", id.type);
		}
	}

	//==========================================================================
	void ResourceManager::destroy_resource(const ResourceId& id)
	{
		switch (id.type)
		{
		case ResourceType::Mesh:
			destroy_resource<Mesh>(id);
			break;

		case ResourceType::Texture:
			destroy_resource<Texture>(id);
			break;

		case ResourceType::Material:
			destroy_resource<MaterialPtr>(id);
			break;

		case ResourceType::Shader:
			destroy_resource<Shader>(id);
			break;

		default:
			LOCO_ASSERTF(false, "Resources of type %d are not handled by the resource manager", id.type);
		}
	}

	//==========================================================================
	void ResourceManager::replace_resource(const ResourceId& id, const Memory* mem)
	{
		switch (id.type)
		{
		case ResourceType::Mesh:
			replace_resource<Mesh>(id, mem);
			break;

		case ResourceType::Texture:
			replace_resource<Texture>(id, mem);
			break;

		case ResourceType::Material:
			replace_resource<MaterialPtr>(id, mem);
			break;

		case ResourceType::Shader:
			replace_resource<Shader>(id, mem);
			break;

		default:
			LOCO_ASSERTF(false, "Resources of type %d are not handled by the resource manager", id.type);
		}
	}
	

} // loco