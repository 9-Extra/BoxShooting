#include "World.h"

World::World() {
	std::fill(entites, entites + ENTITY_MAX, Entity{ EMPTY_MASK });
	valid_id.reserve(ENTITY_MAX - ENTITY_CONSTANT_MAX);

	for (unsigned int i = ENTITY_CONSTANT_MAX; i < ENTITY_MAX; i++)
	{
		valid_id.push_back(i);
	}

	unsigned int seed = (unsigned int)time(NULL);
	debug_log("Set seed: %u\n", seed);
	state.random_engine.seed(seed);
}