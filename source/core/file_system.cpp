
#include "file_system.h"
#include <stdio.h>
#include <string.h>

namespace loco
{
	class File
	{
	public:
		FILE* ff;
	};

	File* FileSystem::open(const char* path, int flags)
	{
		char str_flags[8];
		strcpy(str_flags, "");

		if ((flags & FileSystem::Mode::READ) == FileSystem::Mode::READ)
			strcat(str_flags, "r");

		if ((flags & FileSystem::Mode::WRITE) == FileSystem::Mode::WRITE)
			strcat(str_flags, "w");

		if ((flags & FileSystem::Mode::APPEND) == FileSystem::Mode::APPEND)
			strcat(str_flags, "a");

		if ((flags & FileSystem::Mode::BINARY) == FileSystem::Mode::BINARY)
			strcat(str_flags, "b");

		FILE* ff = fopen(path, str_flags);
		File* file = nullptr;
		if (ff)
		{
			file = new File();
			file->ff = ff;
		}
		return file;
	}

	void FileSystem::close(File* file)
	{
		fclose(file->ff);
		delete file;
	}

	void FileSystem::seek(File* file, size_t position)
	{
		fseek(file->ff, position, SEEK_SET);
	}

	void FileSystem::seek_to_end(File* file)
	{
		fseek(file->ff, 0, SEEK_END);
	}

	size_t FileSystem::tell(File* file)
	{
		return ftell(file->ff);
	}

	size_t FileSystem::read(File* file, void* buffer, size_t length)
	{
		return fread(buffer, 1, length, file->ff);
	}

	size_t FileSystem::wite(File* file, void* buffer, size_t length)
	{
		return fwrite(buffer, 1, length, file->ff);
	}
}
