#include "resource_manager.h"

#include "debug.h"
#include "defines.h"
#include "file_system.h"
#include "file_utils.h"
#include "memory_utils.h"

#include "resource_texture.h"

#include <string>
#include <list>
#include <map>

namespace loco
{
	const ResourceManager::Texture ResourceManager::Texture::invalid = { 0 };

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
	void ResourceManager::init(const char* root_folder_path)
	{
		// init root resources folder
		strcpy(_root_path, root_folder_path);
		_root_path_lenght = strlen(root_folder_path);

		// load common resources
		load_folder(LOCO_COMMON_RESOURCE_PATH);
	}

	//==========================================================================
	unsigned ResourceManager::load_folder(const char* folder_path)
	{
		// concat root path + folder path
		char path[LOCO_PATH_LENGTH];
		strcpy(path, _root_path);
		strcat(path, folder_path);

		// get files in the folder
		std::list<FileInfo> files_infos;
		files_in_directory(path, true, &files_infos);

		//load files
		unsigned loaded_count = 0;
		auto files_infos_it = files_infos.begin();
		while (files_infos_it != files_infos.end())
		{
			loaded_count += load_file(*files_infos_it);
			files_infos_it++;
		}

		return loaded_count;
	}

	//==========================================================================
	bool ResourceManager::load_file(const FileInfo& fi)
	{
		// check the resource type
		ResourceType::Enum type = resource_type(fi);
		if (type == ResourceType::Count)
			return false;

		// check if the file is already loaded
		HashedString name = resource_name(fi);
		auto resource_it = _resources[type].find(name);

		if (resource_it != _resources[type].end())
			return false;

		// ResourceInfo instance
		ResourceInfo ri;
		ri.name = name;
		ri.type = type;
		ri.file_date = fi.last_modif_date;
		_resources[type][name] = ri;

		// create the resource and reference it in the map corresponding to its type
		const Memory* mem = read_file(fi);
		create_resource(ri,mem);
		return true;
	};



	//==========================================================================
	HashedString ResourceManager::resource_name(const char* resource_path) const
	{
		return murmur_hash_64(resource_path, strlen(resource_path));
	}

	//==========================================================================
	HashedString ResourceManager::resource_name(const FileInfo& fi) const
	{
		unsigned path_length = strlen(fi.path);
		unsigned ext_length = strlen(fi.extention);
		unsigned size_to_hash = path_length - _root_path_lenght - ext_length - 1;
		return murmur_hash_64(fi.path + _root_path_lenght, size_to_hash);
	}

	//==========================================================================
	void ResourceManager::unload_all()
	{
		// Destroy resources
		unload_all<Texture>();

		// Clear resources infos tables
		for (int i = 0; i < ResourceType::Count; i++)
		{
			_resources[i].clear();
		}
	}

	//==========================================================================
	ResourceManager::ResourceType::Enum ResourceManager::resource_type(const FileInfo& fi) const
	{
		if (strcmp(fi.extention, "dds") == 0)
			return ResourceManager::ResourceType::Texture;
		else
			return ResourceManager::ResourceType::Count;
	}


	//==========================================================================
	void ResourceManager::create_resource(const ResourceInfo& ri, const Memory* mem)
	{
		switch (ri.type)
		{
		case ResourceType::Texture:
			create_resource<Texture>(ri, mem);
			break;

		default:
			LOCO_ASSERTF(false, "Resources of type %d are not handled by the resource manager", ri.type);
		}
	}

	//==========================================================================
	template<> std::map<HashedString, ResourceManager::Texture>& ResourceManager::resource_map()	{ return _textures; }

} // loco