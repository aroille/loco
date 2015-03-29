#include "file_utils.h"
#include "platform.h"

#include "debug.h"
#include "file_system.h"
#include "log.h"

namespace loco
{
	//==========================================================================
	bool file_read(FileInfo& fi)
	{
		File* file = FileSystem::open(fi.path, FileSystem::ModeFlag::READ | FileSystem::ModeFlag::BINARY);
		if (file == nullptr)
		{
			LOCO_LOG_WARNING("FileSystem", "Can't open file : %s", fi.path);
			return false;
		}
		
		FileSystem::seek_to_end(file);
		size_t file_size = FileSystem::tell(file);
		FileSystem::seek(file, 0);
		if (file_size >= 0)
		{
			fi.mem = alloc(file_size);

			// read file
			size_t readed_size = FileSystem::read(file, fi.mem->data, fi.mem->size);
			if (readed_size != fi.mem->size)
			{
				LOCO_LOG_WARNING("FileSystem", "Error while reading file : %s", fi.path);
				release(fi.mem);
				fi.mem = nullptr;
				FileSystem::close(file);
				return false;
			}
			
		}

		FileSystem::close(file);
		return true;
	}

	//==========================================================================
	void extention(char* file_path, char* result)
	{
		size_t size = strlen(file_path);
		size_t pos = size - 1;
		while ((pos >= 0) && (file_path[pos] != '.'))
			--pos;

		if (pos >= 0)
			strcpy(result, file_path + pos + 1);
		else
			strcpy(result, "");
	}

#ifdef LOCO_PLATFORM_WINDOWS

	//==========================================================================
	unsigned long long file_modification_date(char* file_path)
	{
		WIN32_FILE_ATTRIBUTE_DATA file_attribute_data;
		if (GetFileAttributesEx(file_path, GetFileExInfoStandard, &file_attribute_data))
			return ((unsigned long long)file_attribute_data.ftLastWriteTime.dwHighDateTime << 32) | file_attribute_data.ftLastWriteTime.dwLowDateTime;
		else
			return 0;
	}

	//==========================================================================
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
					fi.last_modif_date = file_modification_date(fi.path);

					// set memory buffer
					fi.mem = nullptr;

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
#endif // LOCO_PLATFORM_WINDOWS

} // namespace loco
