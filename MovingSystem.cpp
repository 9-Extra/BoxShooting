#include "MovingSystem.h"

static const ComponentBitMask MOVING_MASK = CpntMoving::mask() | CpntPosition::mask();

void sys_moving(World& world, const SystemContext& context)
{
	for (unsigned int i = 0; i < ENTITY_MAX; i++) {
		if (mask_contain(world.entites[i].components, MOVING_MASK)) {
			Vector2f& p = world.cpnt_position[i].data;
			if (p.x > 1.0f || p.x < 0.0f || p.y < 0.0f || p.y >= 1.0f) {
				world.destroy_entity(i);
			}
			else {
				p = p + world.cpnt_moving[i].data;
			}
			
		}
	}
}
