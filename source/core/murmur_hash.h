#ifndef MURMUR_HASH_H_HEADER_GUARD
#define MURMUR_HASH_H_HEADER_GUARD

#include "defines.h"
#include "type.h"

#include <string>

namespace loco
{
	typedef uint64 HashedString;

	/// Implementation of the 64 bit MurmurHash2 function
	/// http://murmurhash.googlepages.com/
	uint64 murmur_hash_64(const void *key, uint32 len, uint64 seed = LOCO_MURMUR_HASH_SEED);

	inline HashedString hash_string(const char* string)
	{
		return murmur_hash_64((void*)string, (uint32)strlen(string));
	}
}

#endif // MURMUR_HASH_H_HEADER_GUARD

