#include "resources.h"
#include "defines.h"
#include "murmur_hash.h"
#include "path.h"
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
			char			path[LOCO_PATH_LENGTH];
			char*			data;
			unsigned		data_size;
		};

		static char _root[LOCO_PATH_LENGTH];
		static std::map<unsigned, ResourceInfo> _meshes;

		void init(char* root_folder_path)
		{
			strcpy(_root, root_folder_path);
			resources::load_folder(LOCO_COMMON_RESOURCE_PATH);
		}

		unsigned load_folder(char* folder_path)
		{
			char path[LOCO_PATH_LENGTH];
			strcpy(path, _root);
			strcat(path, folder_path);

			std::list<FileInfo> fi_list;
			files_in_directory(path, true, &fi_list);

			return 0;
		}
	}
}