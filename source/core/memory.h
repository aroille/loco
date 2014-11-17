#ifndef MEMORY_H_HEADER_GUARD
#define MEMORY_H_HEADER_GUARD

#include <stdint.h> // uint32_t

namespace loco
{
	struct Memory
	{
		uint8_t* data;
		uint32_t size;
	};
}

#endif // MEMORY_H_HEADER_GUARD
