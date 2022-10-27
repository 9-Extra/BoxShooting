#include "MovingSystem.h"

static const ComponentBitMask MOVING_MASK = CpntMoving::mask() | CpntPosition::mask();

static const float EDGE_PAD = 5.0f;

void sys_moving(World& world, const SystemContext& context)
{
	float dt = context.dt;
	for (unsigned int i = 0; i < ENTITY_MAX_COUNT; i++) {
		if (mask_contain(world.entites[i].components, MOVING_MASK)) {
			Vector2f& p = world.cpnt_position[i].data;
			if (p.x > WINDOW_WIDTH * MAP_RATIO + EDGE_PAD || p.x < 0.0f - EDGE_PAD || p.y < 0.0f - EDGE_PAD || p.y > WINDOW_HEIGHT * MAP_RATIO + EDGE_PAD) {
				world.destroy_entity(i);
			}
			else {
				p = p + world.cpnt_moving[i].data * dt;
			}
			
		}
	}
}
