#include "resources.h"

#include "debug.h"
#include "defines.h"
#include "file_system.h"
#include "file_utils.h"
#include "memory.h"
#include "murmur_hash.h"
#include "renderer.h"

#include <string>
#include <list>
#include <map>

namespace loco
{
	namespace resources
	{
		struct ResourceInfo
		{
			HashedString	hashed_name;
			Memory*			memory;
		};

		struct Texture	{ HashedString name; };
		struct Mesh		{ HashedString name; };
		struct Shader	{ HashedString name; };
		struct Program	{ HashedString name; };

		struct TextureResource
		{
			renderer::TextureHandle handle;
		};

		struct ShaderResource
		{
			renderer::ShaderHandle handle;
		};

		//==========================================================================
		static FileSystem _file_system;
		
		static char _root[LOCO_PATH_LENGTH];
		static unsigned _root_length = 0;

		typedef std::map<HashedString, ResourceInfo> ResourcesMap;
		static ResourcesMap _resources;

		//==========================================================================
		void init(const char* root_folder_path)
		{
			// init root resources folder
			strcpy(_root, root_folder_path);
			_root_length = strlen(root_folder_path);

			// load common resources
			resources::load_folder(LOCO_COMMON_RESOURCE_PATH);
		}

		//==========================================================================
		HashedString hashed_resource_name(const FileInfo& file_info)
		{
			unsigned path_length = strlen(file_info.path);
			unsigned ext_length = strlen(file_info.extention);
			unsigned size_to_hash = path_length - _root_length - ext_length - 1;
			return murmur_hash_64(file_info.path + _root_length, size_to_hash);
		}

		//==========================================================================
		ResourceInfo load_file(const FileInfo& file_info)
		{
			ResourceInfo resource_info;
			resource_info.hashed_name = hashed_resource_name(file_info);

			File* file = FileSystem::open(file_info.path, FileSystem::Mode::READ | FileSystem::Mode::BINARY);
			LOCO_ASSERTF(file, "Can't open file : %s", file_info.path);
			
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
				LOCO_ASSERTF(readed_size == resource_info.memory->size, "Error while reading file : %s", file_info.path);

				auto resource_it = _resources.find(resource_info.hashed_name);
				LOCO_ASSERTF(resource_it == _resources.end(), "The following file is already loaded : %s", file_info.path);
				_resources[resource_info.hashed_name] = resource_info;
			}
			else
			{
				resource_info.memory = NULL;
			}
			
			FileSystem::close(file);
			//int close_return = fclose(file);
			//LOCO_ASSERTF(close_return == 0, "Can't close file : %s", file_info->path);
			
			return resource_info;
		}

		//==========================================================================
		void create_texture(ResourceInfo resource_info)
		{
			renderer::TextureHandle handle = renderer::create_texture(resource_info.memory);
			renderer::destroy_texture(handle);
		}

		//==========================================================================
		unsigned load_folder(const char* folder_path)
		{
			char path[LOCO_PATH_LENGTH];
			strcpy(path, _root);
			strcat(path, folder_path);

			std::list<FileInfo> files_infos;
			files_in_directory(path, true, &files_infos);

			ResourceInfo resource_info;
			unsigned resource_count = 0;

			auto files_infos_it = files_infos.begin();
			while (files_infos_it != files_infos.end())
			{
				FileInfo& file_info = *files_infos_it;
				if (strcmp(file_info.extention, "dds") == 0)
				{
					resource_info = load_file(file_info);
					create_texture(resource_info);
					resource_count++;
				}

				++files_infos_it;
			}

			return resource_count;
		}

		
	}
}