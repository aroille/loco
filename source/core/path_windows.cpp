#include "path.h"
#include "bx/platform.h"

#ifdef BX_PLATFORM_WINDOWS

#include <Windows.h>

namespace loco
{
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
					//char child_folder_path[LOCO_PATH_LENGTH];
					//wcstombs(child_folder_path, fd.cFileName, LOCO_PATH_LENGTH);
					//names.push_back(fd.cFileName);
					FileInfo fi;
					strcpy(fi.path, folder_path);
					strcat(fi.path, fd.cFileName);
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
}

#endif // BX_PLATFORM_WINDOWS