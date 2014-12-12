#ifndef FILE_UTILS_H_HEADER_GUARD
#define FILE_UTILS_H_HEADER_GUARD

#include "defines.h"
#include "memory_utils.h"
#include <list>

namespace loco
{
	struct FileInfo
	{
		char path[LOCO_PATH_LENGTH];
		char extention[LOCO_EXTENTION_LENGTH];
		unsigned long long last_modif_date;
		const Memory* mem;

		FileInfo& operator =(const FileInfo& fi)
		{
			strcpy(path,fi.path);
			strcpy(extention, fi.extention);
			last_modif_date = fi.last_modif_date;
			mem = fi.mem;
			return *this;
		}
	};

	void files_in_directory(char* folder_path, bool recursive, std::list<FileInfo>* result);

	bool file_read(FileInfo& fi);

	unsigned long long file_modification_date(char* file_path);
}

#endif // FILE_UTILS_H_HEADER_GUARD

