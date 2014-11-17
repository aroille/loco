#ifndef UTILS_H_HEADER_GUARD
#define UTILS_H_HEADER_GUARD

#include <stdint.h>

namespace loco
{
	typedef uint64_t HashedString;

	/// Implementation of the 64 bit MurmurHash2 function
	/// http://murmurhash.googlepages.com/
	uint64_t murmur_hash_64(const void *key, uint32_t len, uint64_t seed);

	HashedString hash(char*);
}

#endif // UTILS_H_HEADER_GUARD

