#ifndef PATH_H_HEADER_GUARD
#define PATH_H_HEADER_GUARD

#include "defines.h"
#include <list>

namespace loco
{
	struct FileInfo
	{
		char path[LOCO_PATH_LENGTH];
		char extention[LOCO_EXTENTION_LENGTH];
	};

	void files_in_directory(char* folder_path, bool recursive, std::list<FileInfo>* result);
}

#endif // PATH_H_HEADER_GUARD

