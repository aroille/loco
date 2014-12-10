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
		return (first.type < second.type);
	}

	//==========================================================================
	const Memory* read_file(const FileInfo& fi)
	{
		const Memory* mem = NULL;

		File* file = FileSystem::open(fi.path, FileSystem::Mode::READ | FileSystem::Mode::BINARY);
		LOCO_ASSERTF(file, "Can't open file : %s", fi.path);

		FileSystem::seek_to_end(file);
		int file_size = FileSystem::tell(file);
		FileSystem::seek(file, 0);
		if (file_size >= 0)
		{
			mem = alloc(file_size);

			// read file
			unsigned readed_size = FileSystem::read(file, mem->data, mem->size);
			LOCO_ASSERTF(readed_size == mem->size, "Error while reading file : %s", fi.path);
		}

		FileSystem::close(file);
		return mem;
	}

	ResourceManager::~ResourceManager()
	{
		unload_all();
	}

	//==========================================================================
	unsigned ResourceManager::load_folder(const char* folder_path)
	{
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
			ri.name = res_name;
			ri.type = type;
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
			loaded_count += load_resource(*res);
			res++;
		}

		return loaded_count;
	}

	//==========================================================================
	bool ResourceManager::load_resource(const ResourceInfo& ri)
	{
		// if resource already loaded, unload previous resource version
		auto fi = _resources[ri.type].find(ri.name);
		if ((fi != _resources[ri.type].end()) && (fi->second.last_modif_date < ri.file_info.last_modif_date))
		{
			destroy_resource(ri);
		}
		
		_resources[ri.type][ri.name] = ri.file_info;

		// create the resource and reference it in the map corresponding to its type
		const Memory* mem = read_file(ri.file_info);
		create_resource(ri,mem);

		return true;
	};

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
		// Destroy resources
		unload_all<Texture>();	
		unload_all<Shader>();
		unload_all<MaterialPtr>();
		unload_all<Mesh>();

		// Clear resources infos tables
		for (int i = 0; i < ResourceType::Count; i++)
			_resources[i].clear();
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
	void ResourceManager::create_resource(const ResourceInfo& ri, const Memory* mem)
	{
		switch (ri.type)
		{
		case ResourceType::Mesh:
			create_resource<Mesh>(ri, mem);
			break;

		case ResourceType::Texture:
			create_resource<Texture>(ri, mem);
			break;

		case ResourceType::Material:
			create_resource<MaterialPtr>(ri, mem);
			break;

		case ResourceType::Shader:
			create_resource<Shader>(ri, mem);
			break;

		default:
			LOCO_ASSERTF(false, "Resources of type %d are not handled by the resource manager", ri.type);
		}
	}

	//==========================================================================
	void ResourceManager::destroy_resource(const ResourceInfo& ri)
	{
		switch (ri.type)
		{
		case ResourceType::Mesh:
			destroy_resource<Mesh>(ri);
			break;

		case ResourceType::Texture:
			destroy_resource<Texture>(ri);
			break;

		case ResourceType::Material:
			destroy_resource<MaterialPtr>(ri);
			break;

		case ResourceType::Shader:
			destroy_resource<Shader>(ri);
			break;

		default:
			LOCO_ASSERTF(false, "Resources of type %d are not handled by the resource manager", ri.type);
		}
	}

	//==========================================================================
	template<> std::map<ResourceName, MaterialPtr>& ResourceManager::resource_map()	{ return _materials; }
	template<> std::map<ResourceName, Mesh>& ResourceManager::resource_map()		{ return _meshes; }
	template<> std::map<ResourceName, Shader>& ResourceManager::resource_map()		{ return _shaders; }
	template<> std::map<ResourceName, Texture>& ResourceManager::resource_map()		{ return _textures; }
} // loco