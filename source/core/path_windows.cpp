#include "path.h"
#include "bx/platform.h"

#ifdef BX_PLATFORM_WINDOWS

#include <Windows.h>

namespace loco
{
	void extention(char* file_path, char* result)
	{
		unsigned size = strlen(file_path);
		unsigned pos = size - 1;
		while ((pos >= 0) && (file_path[pos] != '.'))
			--pos;

		if (pos >= 0)
			strcpy(result, file_path + pos + 1);
		else
			strcpy(result, "");
	}

	unsigned long long last_modification_date(char* file_path)
	{
		WIN32_FILE_ATTRIBUTE_DATA file_attribute_data;
		GetFileAttributesEx(file_path, GetFileExInfoStandard, &file_attribute_data);
		return ((unsigned long long)file_attribute_data.ftLastWriteTime.dwHighDateTime << 32) | file_attribute_data.ftLastWriteTime.dwLowDateTime;
	}

	void files_in_directory(char* folder_path, bool recursive, std::list<FileInfo>* result)
	{
		char search_path[LOCO_PATH_LENGTH];
		strcpy(search_path, folder_path);
		strcat(search_path, "*");

		WIN32_FIND_DATA fd;
		HANDLE hFind = ::FindFirstFile(search_path, &fd);
		if (hFind != INVALID_HANDLE_VALUE)
		{
			do
			{
				if (!(fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
				{
					FileInfo fi;
					// set file path
					strcpy(fi.path, folder_path);
					strcat(fi.path, fd.cFileName);

					// set extention
					extention(fi.path, fi.extention);

					// set last modif date
					fi.last_modif_date = last_modification_date(fi.path);

					result->push_back(fi);
				}
				else if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && 
					(!(fd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)) &&
					(!(fd.dwFileAttributes & FILE_ATTRIBUTE_SYSTEM)) &&
					recursive)
				{
					if ((strcmp(fd.cFileName, ".") != 0) && (strcmp(fd.cFileName, "..") != 0))
					{
						char child_folder_path[LOCO_PATH_LENGTH];
						strcpy(child_folder_path, folder_path);
						strcat(child_folder_path, fd.cFileName);
						strcat(child_folder_path, "/");
						files_in_directory(child_folder_path, true, result);
					}
						
				}
			} while (::FindNextFile(hFind, &fd));
			::FindClose(hFind);
		}
	}



} // namespace loco

#endif // BX_PLATFORM_WINDOWS