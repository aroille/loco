#ifndef MURMUR_HASH_H_HEADER_GUARD
#define MURMUR_HASH_H_HEADER_GUARD

#include "defines.h"

#include <stdint.h>
#include <string>

namespace loco
{
	typedef uint64_t HashedString;

	/// Implementation of the 64 bit MurmurHash2 function
	/// http://murmurhash.googlepages.com/
	uint64_t murmur_hash_64(const void *key, uint32_t len, uint64_t seed = LOCO_MURMUR_HASH_SEED);

	inline HashedString hash_string(const char* string)
	{
		return murmur_hash_64((void*)string, strlen(string));
	}
}

#endif // MURMUR_HASH_H_HEADER_GUARD

