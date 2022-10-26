#include "CollisionSystem.h"

static const ComponentBitMask COLLISION_MASK = CpntCollision::mask() | CpntPosition::mask();

void sys_collision(World& world, const SystemContext& context) {
	for (unsigned int i = 0; i < ENTITY_MAX; i++) {
		if (mask_contain(world.entites[i].components, COLLISION_MASK)) {
			CollisionDesc c_a = world.cpnt_collision[i].data;
			for (unsigned int j = 0; j < ENTITY_MAX; j++) {
				if (mask_contain(world.entites[j].components, COLLISION_MASK))
				{
					CollisionDesc c_b = world.cpnt_collision[j].data;
					if (c_a.mask & c_b.mask) {
						Vector2f p = world.cpnt_position[i].data;
						if (c_a.type == CollsionType::BOX && c_b.type == CollsionType::BOX) {
							


						}
						else {
							debug_log("Unsupport collision type.\n");
						}
					}
				}
			}
		}
	}
}