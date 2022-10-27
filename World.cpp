#include "World.h"

World::World() {
	std::fill(entites, entites + ENTITY_MAX_COUNT, Entity{ EMPTY_MASK });
	
	valid_id_top = 0;
	for (unsigned int i = ENTITY_CONSTANT_COUNT; i < ENTITY_MAX_COUNT; i++)
	{
		valid_id[valid_id_top++] = i;
	}

	unsigned int seed = (unsigned int)time(NULL);
	debug_log("Set seed: %u\n", seed);
	state.random_engine.seed(seed);
}