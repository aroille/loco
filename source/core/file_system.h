#ifndef FILE_SYSTEM_H_HEADER_GUARD
#define FILE_SYSTEM_H_HEADER_GUARD

#include <stdint.h>
#include "defines.h"

namespace loco
{
	class File;

	class FileSystem
	{
	public:

		struct Mode
		{
			enum Enum
			{
				READ = 1,
				WRITE = 2,
				APPEND = 4,
				BINARY = 8,
			};
		};

		/// Open a file using a specific access mode
		static File* open(const char* file_path, int flags);

		/// Close a file
		static void close(File* f);

		/// Seek to position
		static void seek(File* file, size_t position);

		/// Seek to end
		static void seek_to_end(File* file);

		/// returns the current position in the file
		static size_t tell(File* file);

		/// Synchronously reads from a file into the buffer. 
		/// Returns the number of bytes read, 0 if the operation failed
		static size_t read(File* f, void* buffer, size_t length);

		/// Synchronously write from a buffer into the file. 
		/// Returns the number of bytes write, 0 if the operation failed
		static size_t wite(File* f, void* buffer, size_t length);
		
	};
}

#endif // FILE_SYSTEM_H_HEADER_GUARD

