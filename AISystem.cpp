#include "AISystem.h"

static const ComponentBitMask AI_MASK = CpntAI::mask();

void sys_ai_run(World& world, const SystemContext& context) {
	for (unsigned int i = 0; i < ENTITY_MAX_COUNT; i++) {
		if (mask_contain(world.entites[i].components, AI_MASK)) {
			world.cpnt_ai[i].data(world, i);
		}
	}
}