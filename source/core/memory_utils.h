#ifndef MEMORY_H_HEADER_GUARD
#define MEMORY_H_HEADER_GUARD

#include <cstdint> // uint32_t
#include <cstdlib> // malloc & free

namespace loco
{
	struct Memory
	{
		uint8_t* data;
		uint32_t size;
	};

	inline const Memory* alloc(uint32_t size)
	{
		Memory* mem = (Memory*)malloc(sizeof(Memory)+size);
		mem->size = size;
		mem->data = (uint8_t*)mem + sizeof(Memory);
		return mem;
	}

	inline void release(const Memory* mem)
	{
		free(const_cast<Memory*>(mem));
	}
}

#endif // MEMORY_H_HEADER_GUARD
