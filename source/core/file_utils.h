#ifndef FILE_UTILS_H_HEADER_GUARD
#define FILE_UTILS_H_HEADER_GUARD

#include "defines.h"
#include <list>

namespace loco
{
	struct FileInfo
	{
		char path[LOCO_PATH_LENGTH];
		char extention[LOCO_EXTENTION_LENGTH];
		unsigned long long last_modif_date;

		FileInfo& operator =(const FileInfo& fi)
		{
			strcpy(path,fi.path);
			strcpy(extention, fi.extention);
			last_modif_date = fi.last_modif_date;
			return *this;
		}
	};

	void files_in_directory(char* folder_path, bool recursive, std::list<FileInfo>* result);
}

#endif // FILE_UTILS_H_HEADER_GUARD

