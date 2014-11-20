#include "resource_manager.h"

#include "debug.h"
#include "defines.h"
#include "file_system.h"
#include "file_utils.h"
#include "memory.h"
#include "resource_loader.h"

#include <string>
#include <list>
#include <map>

namespace loco
{
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
			char path[LOCO_PATH_LENGTH];
			strcpy(path, _root_path);
			strcat(path, folder_path);

			std::list<FileInfo> files_infos;
			files_in_directory(path, true, &files_infos);

			ResourceInfo resource_info;
			unsigned resource_count = 0;

			auto files_infos_it = files_infos.begin();
			while (files_infos_it != files_infos.end())
			{
				FileInfo& file_info = *files_infos_it;
				ResourceType::Enum type = resource_type(file_info);

				if (type == ResourceType::Unknown) 
				{
					files_infos_it++;
					continue;
				}
				
				resource_info = load_file(file_info);
				//create_resource(type, resource_info);
				switch (type)
				{
					case ResourceType::Texture :
						_textures[resource_info.hashed_name] = load_texture(resource_info);
						break;

					default:
						LOCO_ASSERTF(false, "Resources of type %d can't be handled by the resource manager", type);
				}

				resource_count++;
				files_infos_it++;
			}

			return resource_count;
		}

		//==========================================================================
		ResourceManager::ResourceInfo ResourceManager::load_file(const FileInfo& fi)
		{
			ResourceInfo resource_info;
			resource_info.hashed_name = resource_name(fi);

			auto resource_it = _resources.find(resource_info.hashed_name);
			LOCO_ASSERTF(resource_it == _resources.end(), "The following file is already loaded : %s", fi.path);

			File* file = FileSystem::open(fi.path, FileSystem::Mode::READ | FileSystem::Mode::BINARY);
			LOCO_ASSERTF(file, "Can't open file : %s", fi.path);
			
			FileSystem::seek_to_end(file);
			int file_size = FileSystem::tell(file);
			FileSystem::seek(file, 0);
			if (file_size >= 0)
			{
				resource_info.memory = new Memory();
				resource_info.memory->data = (uint8_t*)malloc(file_size);
				resource_info.memory->size = file_size;

				// read file
				unsigned readed_size = FileSystem::read(file, resource_info.memory->data, resource_info.memory->size);
				LOCO_ASSERTF(readed_size == resource_info.memory->size, "Error while reading file : %s", fi.path);

				_resources[resource_info.hashed_name] = resource_info;
			}
			else
			{
				resource_info.memory = NULL;
			}
			FileSystem::close(file);
			return resource_info;
		}

		//==========================================================================
		HashedString ResourceManager::resource_name(const char* resource_path)
		{
			return murmur_hash_64(resource_path, strlen(resource_path));
		}

		//==========================================================================
		HashedString ResourceManager::resource_name(const FileInfo& fi)
		{
			unsigned path_length = strlen(fi.path);
			unsigned ext_length = strlen(fi.extention);
			unsigned size_to_hash = path_length - _root_path_lenght - ext_length - 1;
			return murmur_hash_64(fi.path + _root_path_lenght, size_to_hash);
		}

		//==========================================================================
		ResourceManager::ResourceType::Enum ResourceManager::resource_type(const FileInfo& fi)
		{
			if (strcmp(fi.extention, "dds") == 0)
				return ResourceType::Texture;
			else
				return ResourceType::Unknown;
		}

		template<> bool ResourceManager::is_loaded<ResourceManager::Texture>(Texture handle)	{ return _textures.count(handle.name) > 0; }
		template<> bool ResourceManager::is_loaded<ResourceManager::Mesh>(Mesh handle)			{ return _meshes.count(handle.name) > 0; }



} // locoR