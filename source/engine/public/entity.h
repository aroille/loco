#ifndef ENTITY_H_HEADER_GUARD
#define ENTITY_H_HEADER_GUARD

namespace loco
{
	const unsigned ENTITY_INDEX_BITS = 24;
	const unsigned ENTITY_INDEX_MASK = (1 << ENTITY_INDEX_BITS) - 1;

	const unsigned ENTITY_GENERATION_BITS = 8;
	const unsigned ENTITY_GENERATION_MASK = (1 << ENTITY_GENERATION_BITS) - 1;


	struct Entity
	{
		unsigned id;

		unsigned index() const { return id & ENTITY_INDEX_MASK; }
		unsigned generation() const { return (id >> ENTITY_INDEX_BITS) & ENTITY_GENERATION_MASK; }
	};
}

#endif // ENTITY_H_HEADER_GUARD