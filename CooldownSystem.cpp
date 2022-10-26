#include "CooldownSystem.h"

static const ComponentBitMask COOLDOWN_MASK = CpntCooldown::mask();

void sys_cooldown(World& world, const SystemContext& context) {
	float dt = context.dt;

	for (unsigned int i = 0; i < ENTITY_MAX; i++) {
		if (mask_contain(world.entites[i].components, COOLDOWN_MASK)) {
			world.cpnt_cooldown[i].data -= dt;
		}
	}
}