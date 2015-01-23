#ifndef MEMORY_H_HEADER_GUARD
#define MEMORY_H_HEADER_GUARD

#include "type.h"
#include <cstdlib> // malloc & free

namespace loco
{
	struct Memory
	{
		uint8* data;
		size_t size;
	};

	inline const Memory* alloc(size_t size)
	{
		Memory* mem = (Memory*)malloc(sizeof(Memory)+size);
		mem->size = size;
		mem->data = (uint8*)mem + sizeof(Memory);
		return mem;
	}

	inline void release(const Memory* mem)
	{
		free(const_cast<Memory*>(mem));
	}
}

#endif // MEMORY_H_HEADER_GUARD
