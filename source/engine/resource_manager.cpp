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
#include "type.h"

#include <string>
#include <list>
#include <map>


namespace loco{
namespace resource{

	//==========================================================================
	bool compare_resource_type(const ResourceManager::ResourceInfo& first, const ResourceManager::ResourceInfo& second)
	{
		return (first.id.type < second.id.type);
	}

	//==========================================================================
	ResourceManager::ResourceManager()
	{
		_default_resources_init = false;

		_default_resources.vertex_shader = Shader::invalid;
		_default_resources.pixel_shader = Shader::invalid;
		_default_resources.texture = Texture::invalid;
		_default_resources.mesh = Mesh::invalid;
		_default_resources.material = Material::invalid;
	}

	//==========================================================================
	ResourceManager::~ResourceManager()
	{
		unload_all();
	}

	//==========================================================================
	void ResourceManager::init_default_resources()
	{
		char resource_path[LOCO_PATH_LENGTH];

		// init default vertex shader
		strcpy(resource_path, loco::default_resource_relativ_path);
		strcat(resource_path, "shader/vs_default");
		_default_resources.vertex_shader = loco::resource_manager.get<Shader>(resource_path);
		LOCO_ASSERTF(!(_default_resources.vertex_shader == Shader::invalid), LOCO_RESOURCE_MANAGER, "Can't load default vertex shader %s", resource_path);

		// init default pixel shader
		strcpy(resource_path, loco::default_resource_relativ_path);
		strcat(resource_path, "shader/ps_default");
		_default_resources.pixel_shader = loco::resource_manager.get<Shader>(resource_path);
		LOCO_ASSERTF(!(_default_resources.pixel_shader == Shader::invalid), LOCO_RESOURCE_MANAGER, "Can't load default pixel shader %s", resource_path);

		// init default texture
		strcpy(resource_path, loco::default_resource_relativ_path);
		strcat(resource_path, "texture/default");
		_default_resources.texture = loco::resource_manager.get<Texture>(resource_path);
		LOCO_ASSERTF(!(_default_resources.texture == Texture::invalid), LOCO_RESOURCE_MANAGER, "Can't load default texture %s", resource_path);

		// init default material
		strcpy(resource_path, loco::default_resource_relativ_path);
		strcat(resource_path, "material/default");
		_default_resources.material = loco::resource_manager.get<Material>(resource_path);
		LOCO_ASSERTF(!(_default_resources.material == Material::invalid), LOCO_RESOURCE_MANAGER, "Can't load default material %s", resource_path);

		// init default mesh
		strcpy(resource_path, loco::default_resource_relativ_path);
		strcat(resource_path, "mesh/bunny");
		_default_resources.mesh = loco::resource_manager.get<Mesh>(resource_path);
		LOCO_ASSERTF(!(_default_resources.mesh == Mesh::invalid), LOCO_RESOURCE_MANAGER, "Can't load default mesh %s", resource_path);

		_default_resources_init = true;
	}

	//==========================================================================
	unsigned ResourceManager::load_folder(const char* folder_path)
	{
		// check if this folder is already loaded
		HashedString root_folder = hash_string(folder_path);
		auto fi = _files.find(root_folder);
		if ((fi != _files.cend()))
		{
			LOCO_ASSERTF(false, LOCO_RESOURCE_MANAGER, "Loading of a folder already loaded : %s", folder_path);
			return 0;
		}

		_files[root_folder] = FileMap();

		// concat root path + folder path
		char path[LOCO_PATH_LENGTH];
		strcpy(path, loco::resource_root_path);
		strcat(path, folder_path);

		// get files in the folder
		std::list<FileInfo> files_infos;
		files_in_directory(path, true, &files_infos);

		// create resource list
		ResourceInfo ri;
		std::list<ResourceInfo> resources;
		for (auto fi = files_infos.begin(); fi != files_infos.end(); fi++)
		{
			ri.id = resource_id(*fi);

			// check the resource type
			if (ri.id.type >= ResourceType::Count)
				continue;

			// check if the file is already loaded
			/*
			auto res = _resources[type].find(res_name);
			if (res != _resources[type].end())
			continue;
			*/

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
			LOCO_ASSERTF(false, LOCO_RESOURCE_MANAGER, "Unloading of a folder not loaded : %s", folder_path);
			return false;
		}

		FileMap& file_map = folder_files_it->second;
		auto file_it = file_map.begin();
		while (file_it != file_map.end())
		{
			unload_resource(file_it->first, folder_path);
			file_it = file_map.begin();
		}

		_files.erase(folder_files_it);

		return true;
	}

	//==========================================================================
	bool ResourceManager::load_resource(ResourceInfo& ri, const HashedString& root_folder)
	{
		LOCO_LOG_INFO(LOCO_RESOURCE_MANAGER, "Loading %s", ri.file_info.path);

		// if resource already loaded, unload previous resource version
		auto fi = _files[root_folder].find(ri.id);
		if ((fi != _files[root_folder].end()))
		{
			LOCO_ASSERTF(false, LOCO_RESOURCE_MANAGER, "The following resource is already loaded : %s", ri.file_info.path);
			return false;
		}

		bool read_success = file_read(ri.file_info);
		if (read_success)
		{
			create_resource(ri.id, ri.file_info.mem);
			_files[root_folder][ri.id] = ri.file_info;

			_memory_to_free.push(MemoryToFree{ ri.file_info.mem, loco::current_frame });
		}
		else
		{
			LOCO_LOG_ERROR(LOCO_RESOURCE_MANAGER, "File reading fail : %s", ri.file_info.path);
		}

		return read_success;
	}

	//==========================================================================
	bool ResourceManager::unload_resource(const ResourceId& id, const HashedString& root_folder)
	{
		// check if the resource is loaded
		auto fi = _files[root_folder].find(id);
		if ((fi == _files[root_folder].end()))
		{
			LOCO_ASSERTF(false, LOCO_RESOURCE_MANAGER, "Unloading a resource not loaded : (%d,&d,%d)", id.name, id.type, root_folder);
			return false;
		}

		destroy_resource(id);
		_files[root_folder].erase(id);

		return true;
	}

	//==========================================================================
	ResourceName ResourceManager::resource_name(const char* resource_path)
	{
		return hash_string(resource_path);
	}

	//==========================================================================
	ResourceManager::ResourceId ResourceManager::resource_id(const FileInfo& fi)
	{
		ResourceId id;

		size_t path_length = strlen(fi.path);
		size_t ext_length = strlen(fi.extention);
		uint32 size_to_hash = (uint32)(path_length - resource_root_path_length - ext_length - 1);

		id.name = murmur_hash_64(fi.path + resource_root_path_length, size_to_hash);

		if (strcmp(fi.extention, "dds") == 0)
			id.type = ResourceManager::ResourceType::Texture;
		else if (strcmp(fi.extention, "mesh") == 0)
			id.type = ResourceManager::ResourceType::Mesh;
		else if (strcmp(fi.extention, "material") == 0)
			id.type = ResourceManager::ResourceType::Material;
		else if (strcmp(fi.extention, backend::shader_extention()) == 0)
			id.type = ResourceManager::ResourceType::Shader;
		else
			id.type = ResourceManager::ResourceType::Unknown;

		return id;
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
			create_resource<Material>(id, mem);
			break;

		case ResourceType::Shader:
			create_resource<Shader>(id, mem);
			break;

		default:
			LOCO_ASSERTF(false, LOCO_RESOURCE_MANAGER, "Resources of type %d are not handled by the resource manager", id.type);
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
			destroy_resource<Material>(id);
			break;

		case ResourceType::Shader:
			destroy_resource<Shader>(id);
			break;

		default:
			LOCO_ASSERTF(false, LOCO_RESOURCE_MANAGER, "Resources of type %d are not handled by the resource manager", id.type);
		}
	}

	//==========================================================================
	void ResourceManager::free_memory()
	{
		while (!_memory_to_free.empty())
		{
			if ((loco::current_frame - _memory_to_free.front().frame) > 2)
			{
				release(_memory_to_free.front().mem);
				_memory_to_free.pop();
			}
			else
			{
				break;
			}
		}
	}

} // resource
} // loco